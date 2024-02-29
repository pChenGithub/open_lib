#include "socket_file.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define FFR_CODE_OK 0
#define FFR_CODE_ERR_SIZE 1 // 文件大小信息异常
#define FFR_CODE_ERR_INDEX 2 // 文件帧序号异常

typedef struct {
    long filesize;
    int framecount;
    int laseframesize;
    int index;
    int code;
} FILE_FRAME_RSP;

#define FRAME_BUFF_SIZE 1024
typedef struct {
    long filesize;
    int framecount;
    int laseframesize;
    // 变量
    int index;
    unsigned char buff[FRAME_BUFF_SIZE];
} FILE_FRAME;

#if __WIN32
int sock_send_file(SOCKET socketfd, const char* file) {
#else
int sock_send_file(int socketfd, const char* file) {
#endif
    int ret = 0;
    if (NULL==file)
        return -TCPIPERR_CHECK_PARAM;

    // 打开文件，统计大小，
    FILE* fp = fopen(file, "rb");
    if (NULL==fp)
        return -TCPIPERR_OPENFILE;

    struct stat file_stat;
    ret = stat(file, &file_stat);
    if (ret<0) {
        ret = -TCPIPERR_FSTAT;
        goto close_exit;
    }

    // 准备发送帧
    FILE_FRAME* fframe = calloc(1, sizeof(FILE_FRAME));
    fframe->filesize = file_stat.st_size;
    fframe->framecount = fframe->filesize/FRAME_BUFF_SIZE;
    fframe->laseframesize = fframe->filesize % FRAME_BUFF_SIZE;
    if (fframe->laseframesize)
        fframe->framecount++;

    printf("size %d, lastsize %d, count %d\n", fframe->filesize, fframe->laseframesize, fframe->framecount);
    int sendsize = sizeof(FILE_FRAME);
    int readsize = FRAME_BUFF_SIZE;
    if (1==fframe->framecount)
        readsize = fframe->laseframesize;
    // 准备读取文件，发送
    printf("readsize %d\n", readsize);
    while((ret=fread(fframe->buff, readsize, 1, fp))>0) {
        if ((fframe->index+1)==fframe->framecount)
            sendsize = fframe->laseframesize+(fframe->buff-(unsigned char*)fframe);
        //printf("send file data, size %d ret %d\n", sendsize, ret);
        //printf("framecount %d\n", fframe->framecount);
        ret = send(socketfd, (const char*)fframe, sendsize, 0);
        if (ret<0) {
            ret = -TCPIPERR_SEND_DATA;
            goto free_exit;
        }

#if 1
        // 等待回复，设置超时
        FILE_FRAME_RSP rsp;
        ret = recv(socketfd, (char*)(&rsp), sizeof(FILE_FRAME_RSP), 0);
        if (ret<0) {
            ret = -TCPIPERR_RECV_DATA;
            goto free_exit;
        }

        //printf("rsp file send framecount %d filesize %d index %d code %d\n", rsp.framecount, rsp.filesize, rsp.index, rsp.code);
#endif

        // 校验回复
        if (fframe->filesize!=rsp.filesize || fframe->framecount!=rsp.framecount
                || fframe->laseframesize!=rsp.laseframesize) {
            ret = -TCPIPERR_RSP;
            goto free_exit;
        }

        if (rsp.index!=fframe->index) {
            ret = -TCPIPERR_RSP_INDEX;
            goto free_exit;
        }

        if (FFR_CODE_OK!=rsp.code) {
            ret = -TCPIPERR_RSP_CODE;
            goto free_exit;
        }

        // 发送的index从0开始
        fframe->index ++;
        if ((fframe->index+1)==fframe->framecount)
            readsize = fframe->laseframesize;
    }

    // 跳出循环后，如果ret为0，表示文件读取完了，
    // ret<0 表示读文件异常
    printf("over read file ret %d\n", ret);
    if (ret<0) {
        // 读取文件失败
        ret = -TCPIPERR_READ_FILE;
    }

    // 发送文件完成
free_exit:
    free(fframe);
close_exit:
    fclose(fp);
    return ret;
}

#if __WIN32
int sock_recv_file(SOCKET socketfd, const char* file) {
#else
int sock_recv_file(int socketfd, const char* file) {
#endif
    int ret = 0;
    unsigned int datasize = 0;
    if (NULL==file)
        return -TCPIPERR_CHECK_PARAM;

    // 打开文件
    FILE* fp = fopen(file, "wb");
    if (NULL==fp)
        return -TCPIPERR_OPENFILE;

    // 准备接收帧
    FILE_FRAME* fframe = calloc(1, sizeof(FILE_FRAME));
    FILE_FRAME_RSP rsp = {
        // 在接到第一帧数据之前，前面3个参数未知，index为0，期待接到序号为0的帧
        0,0,0,0,FFR_CODE_OK,
    };
    // 等待数据，超时？？
    do {
        ret = recv(socketfd, (char*)fframe, sizeof(FILE_FRAME), 0);
        //printf("recv data, size %d\n", ret);
        if (ret<0) {
            ret = -TCPIPERR_RECV_DATA;
            goto free_exit;
        }

        datasize = ret-(fframe->buff-(unsigned char*)fframe);
        // index为0的帧初始化参数
        if (0==fframe->index) {
            rsp.filesize = fframe->filesize;
            rsp.framecount = fframe->framecount;
            rsp.laseframesize = fframe->laseframesize;
        }

        // 校验帧
        if (fframe->filesize!=rsp.filesize || fframe->framecount!=rsp.framecount
                || fframe->laseframesize!=rsp.laseframesize) {
            rsp.code = FFR_CODE_ERR_SIZE;
            send(socketfd, (const char*)&rsp, sizeof(FILE_FRAME_RSP), 0);

            ret = -TCPIPERR_RSP;
            goto free_exit;
        }

        //printf("except %d, come %d, count %d\n", rsp.index, fframe->index, fframe->framecount);
        if (rsp.index!=fframe->index) {
            rsp.code = FFR_CODE_ERR_INDEX;
            send(socketfd, (const char*)&rsp, sizeof(FILE_FRAME_RSP), 0);

            ret = -TCPIPERR_RSP_INDEX;
            goto free_exit;
        }

        // 写文件
        ret = fwrite(fframe->buff, datasize, 1, fp);
        if (ret<0) {
            ret = -TCPIPERR_WRITE_FILE;
            goto free_exit;
        }

        // 回复
        ret = send(socketfd, (const char*)&rsp, sizeof(FILE_FRAME_RSP), 0);
        if (ret<0) {
            ret = -TCPIPERR_SEND_DATA;
            goto free_exit;
        }

        // 期待下一帧
        rsp.index ++;
        if (rsp.index==fframe->framecount) {
            // 所有帧发送结束
            ret = 0;
            break;
        }
    } while(1);

free_exit:
    free(fframe);
//close_exit:
    fclose(fp);
    return ret;
}

int ip_send_file(const char *ip, int port, const char *file)
{
#if __WIN32
    SOCKET socketfd = 0;
#else
    int socketfd = 0;
#endif
    int ret = 0;
    struct sockaddr_in ServerAddr;

    if (NULL==ip || NULL==file)
        return -TCPIPERR_CHECK_PARAM;

#if __WIN32
    WORD sockVersion=MAKEWORD(2,2);
    WSADATA wsaData;//WSADATA结构体变量的地址值
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        printf("WSAStartup() error!");
        return 0;
    }
#endif

    // 打开socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
#if __WIN32
    if (INVALID_SOCKET==socketfd) {
    WSACleanup();
#else
    if (socketfd<0) {
#endif
        printf("socket create error, errno %d\n", errno);
        return -TCPIPERR_SOCKET_CREATE;
    }

    // 连接服务端
    /* 配置服务器IP、端口信息 */
    memset(&ServerAddr, 0, sizeof(struct sockaddr));	//每一个字节都用0来填充
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    ServerAddr.sin_addr.s_addr = inet_addr(ip);

    // 绑定socket和服务ip端口
    ret = connect(socketfd, (struct sockaddr*)& ServerAddr, sizeof(struct sockaddr));
#if __WIN32
    if (ret == SOCKET_ERROR) {
#else
    if (-1==ret) {
#endif
        printf("connect error\n");
        ret = -TCPIPERR_CONNECT;
        goto error_set;
    }

    ret = sock_send_file(socketfd, file);
    if (ret<0)
        printf("send file error, %d\n", ret);
error_set:
#if __WIN32
    closesocket(socketfd);
    WSACleanup();
#else
    close(socketfd);
#endif
    return ret;
}

int ip_recv_file(const char *ip, int port, const char *file)
{
#if __WIN32
    SOCKET socketfd = 0;
#else
    int socketfd = 0;
#endif
    int ret = 0;
    struct sockaddr_in ServerAddr;

    if (NULL==ip || NULL==file)
        return -TCPIPERR_CHECK_PARAM;

#if __WIN32
    WORD sockVersion=MAKEWORD(2,2);
    WSADATA wsaData;//WSADATA结构体变量的地址值
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        printf("WSAStartup() error!");
        return 0;
    }
#endif

    // 打开socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
#if __WIN32
    if (INVALID_SOCKET==socketfd) {
    WSACleanup();
#else
    if (socketfd<0) {
#endif
        printf("socket create error, errno %d\n", errno);
        return -TCPIPERR_SOCKET_CREATE;
    }

    // 连接服务端
    /* 配置服务器IP、端口信息 */
    memset(&ServerAddr, 0, sizeof(struct sockaddr));	//每一个字节都用0来填充
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    ServerAddr.sin_addr.s_addr = inet_addr(ip);

    // 绑定socket和服务ip端口
    ret = connect(socketfd, (struct sockaddr*)& ServerAddr, sizeof(struct sockaddr));
#if __WIN32
    if (ret == SOCKET_ERROR) {
#else
    if (-1==ret) {
#endif
        printf("connect error\n");
        ret = -TCPIPERR_CONNECT;
        goto error_set;
    }

    ret = sock_recv_file(socketfd, file);
    if (ret<0)
        printf("recv file error, %d\n", ret);
error_set:
#if __WIN32
    closesocket(socketfd);
    WSACleanup();
#else
    close(socketfd);
#endif
    return ret;
}

// 等待tcp连接线程
static void *waitTcpMsg(void* arg) {
    // 创建线程,等待组播消息
    printf("接收线程开始\n");
    int ret = 0;
#if __WIN32
    SOCKET msgfd = 0;
#else
    int msgfd = 0;
#endif
    RECV_TCP_MSG_BODY *body = (RECV_TCP_MSG_BODY *)arg;
    handTcpArg* callArg = &(body->callbackArg);
    // 设置线程名字
    while (1) {
        callArg->len = sizeof(struct sockaddr_in);
        printf("wait client connect\n");
        msgfd = accept(body->socketfd, (struct sockaddr*)&(callArg->srcaddr), &callArg->len);
        if (msgfd<0) {
            printf("a connect error\n");
            continue ;
        }

        callArg->msgfd = msgfd;
        if (NULL!=body->fn) {
            ret = body->fn(callArg);
            // 释放fn
            body->fn = NULL;
        }
        close(msgfd);
        printf("callback over, ret %d errno %d\n", ret, errno);
    }
}

int tcp_listen_start(RECV_TCP_MSG_BODY** entry, handTcpMsg callback, char* serverip, int port) {
#if __WIN32
    SOCKET socketfd = 0;
#else
    int socketfd = 0;
#endif
    int ret = 0;
    if (NULL==entry || NULL==serverip)
        return -TCPIPERR_CHECK_PARAM;

    RECV_TCP_MSG_BODY* body = (RECV_TCP_MSG_BODY*)calloc(sizeof(RECV_TCP_MSG_BODY), 1);
    if (NULL==body) {
        return -TCPIPERR_MALLOCA;
    }

    // 创建套接字
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
#if __WIN32
    if (INVALID_SOCKET==socketfd) {
#else
    if (-1==socketfd) {
#endif
        printf("创建套接字失败, errno %d\n", errno);
        ret = -TCPIPERR_SOCKET_CREATE;
        goto error_malloc;
    }

     // 绑定地址,本地ip
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);    // 大端
    addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 0.0.0.0
    ret = bind(socketfd, (struct sockaddr*)&addr, sizeof(addr));
    if (-1==ret) {
        printf("绑定IP失败, errno %d\n", errno);
        ret = -TCPIPERR_SOCKET_BIND;
        goto error_bind;
    }

    /* 启动监听 */
	ret = listen(socketfd, 1);
	if (ret<0) {
		printf("listen socket error\n");
        ret = -TCPIPERR_SOCKET_LISTEN;
		goto error_bind;
	}

    // 
    body->socketfd = socketfd;
    body->fn = callback;
    // 接收端,等待接受数据
    ret = pthread_create(&(body->pid), NULL, waitTcpMsg, body);
    if (ret<0) {
        ret = -TCPIPERR_PTREAD_CREAT;
         goto error_bind;
    }
    //
    pthread_detach(body->pid);

    // 返回
    *entry = body;
    return 0;

error_bind:
#if __WIN32
    closesocket(socketfd);
#else
    close(socketfd);
#endif
error_malloc:
    free(body);
    body = NULL;
    return ret;
}

int tcp_listen_del(RECV_TCP_MSG_BODY* entry) {
    if (NULL==entry)
        return -TCPIPERR_CHECK_PARAM;
    
    // 关闭线程
    pthread_cancel(entry->pid);
    // 等待线程退出
    pthread_join(entry->pid, NULL);
    // 关闭fd
#if __WIN32
    closesocket(entry->socketfd);
#else
    close(entry->socketfd);
#endif
    // 销毁内存
    free(entry);
    return 0;
}

