#include "multicast.h"
#include "tcpIp_errno.h"
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// https://blog.csdn.net/weixin_44522306/article/details/119680394
#if 0
struct ip_mreq
{
        struct in_addr imr_multiaddr;
        struct in_addr imr_interface;
};
#endif

static void *waitMunlticaseMsg(void* arg) {
    // 创建线程,等待组播消息
    printf("接收线程开始\n");
    int ret = 0;
    RECV_MSG_BODY *body = (RECV_MSG_BODY *)arg;
    handMulticastArg* callArg = &(body->callbackArg);
    // 设置线程名字
    while (1) {
        //printf("等待消息\n");
        callArg->len = sizeof(struct sockaddr_in);
        //ret = recvfrom(callArg->socketfd, callArg->recvBuff, sizeof(callArg->recvBuff), 0, NULL, NULL);
        ret = recvfrom(callArg->socketfd, callArg->recvBuff, sizeof(callArg->recvBuff), 0, (struct sockaddr *)(&(callArg->srcaddr)), &(callArg->len));
        if (-1==ret) {
            printf("接收消息失败, 错误码 %d\n", errno);
            continue;
        }

        printf("获取到消息 %s, 地址 %s\n", callArg->recvBuff, inet_ntoa(callArg->srcaddr.sin_addr));

        if (NULL!=body->fn) {
            body->fn(callArg);
        }
    }

    return NULL;
}

int multicast_listen_start(RECV_MSG_BODY** entry, handMultiaddrMst callback, char* groupIp, int port) {
    int ret = 0;
#if __WIN32
    SOCKET socketfd = 0;
#else
    int socketfd = 0;
#endif
    //pthread_t pid;

    if (NULL==entry || NULL==groupIp)
        return -TCPIPERR_CHECK_PARAM;

    // 检查IP的合法性 groupIp

    //
    RECV_MSG_BODY* body = (RECV_MSG_BODY*)calloc(sizeof(RECV_MSG_BODY), 1);
    if (NULL==body) {
        return -TCPIPERR_MALLOCA;
    }

#if __WIN32
    WORD sockVersion=MAKEWORD(2,2);
    WSADATA wsaData;//WSADATA结构体变量的地址值
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        printf("WSAStartup() error!");
        return 0;
    }
#endif

    // 创建套接字
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
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
#if __WIN32
    if (SOCKET_ERROR==ret) {
#else
    if (-1==ret) {
#endif
        printf("绑定IP失败, errno %d\n", errno);
        ret = -TCPIPERR_SOCKET_BIND;
        goto error_bind;
    }

    // 设置组播组
    /** 
     * Hosts可以在任何时间加入或者离开组播组，对于组播组的成员没有所处位置的限制，也没有数量的限制，
     * D类互联网地址是用于组播的：224.0.0.0 - 239.255.255.255。
    */
#if 0
    struct ip_mreqn opt;
    // 要加入到哪个多播组, 通过组播地址来区分
    inet_pton(AF_INET, "224.0.1.0", &opt.imr_multiaddr.s_addr);
    opt.imr_address.s_addr = htonl(INADDR_ANY);
    opt.imr_ifindex = if_nametoindex("ens33");
    ret = setsockopt(socketfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &opt, sizeof(opt));
    if (-1==ret) {
        printf("加入组播组失败, errno %d\n", errno);
        ret = -TCPIPERR_SOCKET_SETSCOKOPT;
        goto error_bind;
    }
#else
    // 注意加入这个组 224.0.1.0
    struct ip_mreq mreq; // 多播地址结构体
    memset((void*)&mreq, 0, sizeof(struct ip_mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(groupIp);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);	
    // 加入组
    ret = setsockopt(socketfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
#if __WIN32
    if (SOCKET_ERROR==ret) {
#else
    if (-1==ret) {
#endif
        printf("加入组播组失败, errno %d\n", errno);
        ret = -TCPIPERR_SOCKET_SETSCOKOPT;
        goto error_bind;
    }
#endif

    body->callbackArg.socketfd = socketfd;
    body->fn = callback;
    // 接收端,等待接受数据
    ret = pthread_create(&(body->pid), NULL, waitMunlticaseMsg, body);
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
    WSACleanup();
#else
    close(socketfd);
#endif
error_malloc:
    free(body);
    body = NULL;
    return ret;
}

int multicast_listen_del(RECV_MSG_BODY* entry) {
    if (NULL==entry)
        return -TCPIPERR_CHECK_PARAM;
    
    // 关闭线程
    pthread_cancel(entry->pid);
    // 等待线程退出
    pthread_join(entry->pid, NULL);
    // 关闭fd
#if __WIN32
    closesocket(entry->callbackArg.socketfd);
    WSACleanup();
#else
    close(entry->callbackArg.socketfd);
#endif
    // 销毁内存
    free(entry);
    return 0;
}

int multicast_resp(RECV_MSG_BODY* entry, char* buff, int len) {
    int ret = 0;
    if (NULL == entry || NULL == buff || len <= 0)
        return -TCPIPERR_CHECK_PARAM;

        #if 1
    // 数据广播
    ret = sendto(entry->callbackArg.socketfd, buff, len, 0, (struct sockaddr *)(&(entry->callbackArg.srcaddr)), entry->callbackArg.len);
    if (-1==ret) {
        printf("发送组播失败, errno %d\n", errno);
        ret = -TCPIPERR_SENDMSG;
    }
    #endif

    return 0;
}

int multicast_sendmsg(char* buff, int bufflen, int sendsize, char* groupIp, int port) {
    return multicast_sendmsg_wait(buff, bufflen, sendsize, groupIp, port, NULL, 0);
}

int multicast_sendmsg_wait(char* buff, int bufflen, int sendsize,
                           char* groupIp, int port,  handMulticastRsp callbk, unsigned int ms) {
#if __WIN32
    SOCKET socketfd = 0;
#else
    int socketfd = 0;
#endif
    int ret = 0;
    int selret = 0;

    if (NULL==buff || bufflen<=0 || NULL==groupIp)
        return -TCPIPERR_CHECK_PARAM;
    // 检查IP的合法性

#if __WIN32
    WORD sockVersion=MAKEWORD(2,2);
    WSADATA wsaData;//WSADATA结构体变量的地址值
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        printf("WSAStartup() error!");
        return 0;
    }
#endif

    // 创建套接字
    //socketfd = socket(AF_INET, SOCK_DGRAM|SOCK_NONBLOCK, 0);
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
#if __WIN32
    if (INVALID_SOCKET==socketfd) {
        WSACleanup();
#else
    if (socketfd<0) {
#endif
        printf("创建套接字失败, errno %d\n", errno);
        return -TCPIPERR_SOCKET_CREATE;
    }
    // 设置为非阻塞

    // 设置同主机还是跨主机
    //int iFlag = 1;	// 0-同一台主机 1-夸主机
    //ret = setsockopt(socketfd, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&iFlag, sizeof(iFlag));

    //  设置发送地址,地址即是组播地址
    struct sockaddr_in cliaddr;
    memset(&cliaddr,0,sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(port); // 接收端需要绑定9999端口
    // 发送组播消息, 需要使用组播地址, 和设置组播属性使用的组播地址一致就可以
    inet_pton(AF_INET, groupIp, &cliaddr.sin_addr.s_addr);

    // 数据广播
    ret = sendto(socketfd, buff, sendsize, 0, (struct sockaddr*)(&cliaddr), sizeof(struct sockaddr));
    if (-1==ret) {
        printf("发送组播失败, errno %d\n", errno);
        ret = -TCPIPERR_SENDMSG;
    }

#if 0
#if 0
    if (ms>0) {
        // 指定了等待时间,等待回复消息,这里如果不需要知道发送端地址,都指定为空
        ret = recvfrom(socketfd, buff, len, 0, NULL, NULL);
        if (-1==ret) {
            printf("接收消息失败, errno %d\n", errno);
            ret = -TCPIPERR_RECVMSG;
        }
    }
    #else
    while ((ret=recvfrom(socketfd, buff, len, 0, NULL, NULL))>0)
    {
        printf("获取消息 %s, ret %d\n", buff, ret);
    }
    printf("<== ret %d\n", ret);
#endif
#endif

    fd_set rfds;
    struct timeval tv;
    FD_ZERO(&rfds);
    FD_SET(socketfd, &rfds);
    // 设置超时时间
    tv.tv_sec = 0;
    tv.tv_usec = 10000;

    // 预计读取数据为超时
    ret = -TCPIPERR_WAITRECV_TIMEOUT;
start_select:
    selret = select(socketfd+1, &rfds, NULL, NULL, &tv);
    if (-1==selret) {
        ret = -TCPIPERR_SELECT;
    } else if (0 == selret) {
        printf("wait data timeout\n");
        //ret = -TCPIPERR_WAITRECV_TIMEOUT;
    } else {
        // >0,有数据
        ret = recvfrom(socketfd, buff, bufflen, 0, NULL, NULL);
        if (-1==ret) {
            ret = -TCPIPERR_RECVMSG;
            goto error_set;
        }
        // printf("获取消息 %s, ret %d\n", buff, ret);

        if (ret>0 && NULL!=callbk) {
            callbk(buff, ret);
        }
        // 标记读取成功,
        ret = 0;
        // 重新监听是否有数据返回
        goto start_select;
    }

error_set:
#if __WIN32
    closesocket(socketfd);
    WSACleanup();
#else
    close(socketfd);
#endif
    return ret;
}

int udp_sendmsg_wait(char* buff, int bufflen, int sendsize,
                     char* ip, int port, handMulticastRsp callbk, unsigned int ms) {
#if __WIN32
    SOCKET socketfd = 0;
#else
    int socketfd = 0;
#endif
    int ret = 0;
    int selret = 0;

    if (NULL==buff || bufflen<=0 || NULL==ip)
        return -TCPIPERR_CHECK_PARAM;
    // 检查IP的合法性

#if __WIN32
    WORD sockVersion=MAKEWORD(2,2);
    WSADATA wsaData;//WSADATA结构体变量的地址值
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        printf("WSAStartup() error!");
        return 0;
    }
#endif

    // 创建套接字
    //socketfd = socket(AF_INET, SOCK_DGRAM|SOCK_NONBLOCK, 0);
    socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#if __WIN32
    if (INVALID_SOCKET==socketfd) {
    WSACleanup();
#else
    if (socketfd<0) {
#endif
        printf("创建套接字失败, errno %d\n", errno);
        return -TCPIPERR_SOCKET_CREATE;
    }
    // 设置为非阻塞

    //  设置发送地址,地址即是组播地址
    struct sockaddr_in cliaddr;
    memset(&cliaddr,0,sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(port); // 接收端需要绑定9999端口
    // 发送组播消息, 需要使用组播地址, 和设置组播属性使用的组播地址一致就可以
    inet_pton(AF_INET, ip, &cliaddr.sin_addr.s_addr);

    // 数据广播
    ret = sendto(socketfd, buff, sendsize, 0, (struct sockaddr*)(&cliaddr), sizeof(struct sockaddr));
    if (ret<=0) {
        printf("sendto error, errno %d\n", errno);
        ret = -TCPIPERR_SENDMSG;
    }

    fd_set rfds;
    struct timeval tv;
    FD_ZERO(&rfds);
    FD_SET(socketfd, &rfds);
    // 设置超时时间
    tv.tv_sec = 0;
    tv.tv_usec = 10000;

    // 预计读取数据为超时
    ret = -TCPIPERR_WAITRECV_TIMEOUT;
start_select:
    selret = select(socketfd+1, &rfds, NULL, NULL, &tv);
    if (-1==selret) {
        ret = -TCPIPERR_SELECT;
    } else if (0 == selret) {
        printf("wait data timeout\n");
        //ret = -TCPIPERR_WAITRECV_TIMEOUT;
    } else {
        // >0,有数据
        ret = recvfrom(socketfd, buff, bufflen, 0, NULL, NULL);
        if (-1==ret) {
            ret = -TCPIPERR_RECVMSG;
            goto error_set;
        }
        // printf("获取消息 %s, ret %d\n", buff, ret);

        if (ret>0 && NULL!=callbk) {
            ret = callbk(buff, ret);
        }

        if (ret<0) {
            ret = -TCPIPERR_CALLBACK;
            goto error_set;
        }

        // 标记读取成功,
        ret = 0;
        // 重新监听是否有数据返回
        goto start_select;
    }

error_set:
#if __WIN32
    closesocket(socketfd);
    WSACleanup();
#else
    close(socketfd);
#endif
    return ret;
}


