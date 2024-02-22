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

int sock_send_file(int socketfd, const char* file) {
    int ret = 0;
    if (NULL==file)
        return -TCPIPERR_CHECK_PARAM;

    // 打开文件，统计大小，
    int fd = open(file, O_RDWR);
    if (fd<0)
        return -TCPIPERR_OPENFILE;

    struct stat file_stat;
    ret = fstat(fd, &file_stat);
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

    int sendsize = sizeof(FILE_FRAME);
    // 准备读取文件，发送
    while((ret=read(fd, fframe->buff, FRAME_BUFF_SIZE))>0) {
        if (FRAME_BUFF_SIZE!=ret)
            sendsize = fframe->buff-(unsigned char*)fframe + ret;
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
    }

    if (ret<0) {
        // 读取文件失败
        ret = -TCPIPERR_READ_FILE;
    }

    // 发送文件完成
free_exit:
    free(fframe);
close_exit:
    close(fd);
    return ret;
}

int sock_recv_file(int socketfd, const char* file) {
    int ret = 0;
    int datasize = 0;
    if (NULL==file)
        return -TCPIPERR_CHECK_PARAM;

    // 打开文件
    int fd = open(file, O_RDWR|O_CREAT, 0666);
    if (fd<0)
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
        printf("recv data, size %d\n", ret);
        if (ret<=0) {
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

        if (rsp.index!=fframe->index) {
            rsp.code = FFR_CODE_ERR_INDEX;
            send(socketfd, (const char*)&rsp, sizeof(FILE_FRAME_RSP), 0);

            ret = -TCPIPERR_RSP_INDEX;
            goto free_exit;
        }

        // 写文件
        ret = write(fd, fframe->buff, datasize);
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
    } while(1);

free_exit:
    free(fframe);
//close_exit:
    close(fd);
    return ret;
}

// 等待tcp连接线程
static void *waitTcpMsg(void* arg) {
    // 创建线程,等待组播消息
    printf("接收线程开始\n");
    int ret = 0;
    int msgfd = 0;
    RECV_TCP_MSG_BODY *body = (RECV_TCP_MSG_BODY *)arg;
    handTcpArg* callArg = &(body->callbackArg);
    // 设置线程名字
    while (1) {
        callArg->len = sizeof(struct sockaddr_in);
        printf("wait client connect\n");
        msgfd = accept(callArg->socketfd, (struct sockaddr*)&(callArg->srcaddr), &callArg->len);
        if (msgfd<0) {
            printf("a connect error\n");
            continue ;
        }

        ret = sock_recv_file(msgfd, "/data/upupupup.tar.gz");
#if 0
        while((ret = recv(msgfd, callArg->recvBuff, sizeof(callArg->recvBuff), 0))>0) {
            if (NULL!=body->fn) {
                body->fn(callArg);
            }
        }
#endif
        printf("recv data over, ret %d errno %d\n", ret, errno);
    }
}

int tcp_listen_start(RECV_TCP_MSG_BODY** entry, handTcpMsg callback, char* serverip, int port) {
    int socketfd = 0;
    int ret = 0;
    if (NULL==entry || NULL==serverip)
        return -TCPIPERR_CHECK_PARAM;

    RECV_TCP_MSG_BODY* body = (RECV_TCP_MSG_BODY*)calloc(sizeof(RECV_TCP_MSG_BODY), 1);
    if (NULL==body) {
        return -TCPIPERR_MALLOCA;
    }

    // 创建套接字
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1==socketfd) {
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
    body->callbackArg.socketfd = socketfd;
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
    close(socketfd);
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
    close(entry->callbackArg.socketfd);
    // 销毁内存
    free(entry);
    return 0;
}
