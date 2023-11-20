#include "multicast.h"
#include "tcpIp_errno.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
        // ret = recvfrom(callArg->socketfd, callArg->recvBuff, sizeof(callArg->recvBuff), 0, (struct sockaddr *)&sendaddr, &len);
        ret = recvfrom(callArg->socketfd, callArg->recvBuff, sizeof(callArg->recvBuff), 0, NULL, NULL);
        if (-1==ret) {
            printf("接收消息失败, 错误码 %d\n", errno);
            continue;
        }

        //printf("获取到消息 %s\n", callArg->recvBuff);

        if (NULL!=body->fn) {
            body->fn(callArg);
        }
    }

    return NULL;
}

int multicast_listen_start(RECV_MSG_BODY** entry, handMultiaddrMst callback, char* groupIp, int port) {
    int ret = 0;
    int socketfd = 0;
    //pthread_t pid;

    if (NULL==entry || NULL==groupIp)
        return -TCPIPERR_CHECK_PARAM;

    // 检查IP的合法性 groupIp

    //
    RECV_MSG_BODY* body = (RECV_MSG_BODY*)calloc(sizeof(RECV_MSG_BODY), 1);
    if (NULL==body) {
        return -TCPIPERR_MALLOCA;
    }

    // 创建套接字
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
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
    memset(&mreq, 0, sizeof(struct ip_mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(groupIp);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);	
    // 加入组
	ret = setsockopt(socketfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if (-1==ret) {
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
    close(socketfd);
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
    close(entry->callbackArg.socketfd);
    // 销毁内存
    free(entry);
    return 0;
}

int multicast_sendmsg(char* buff, int len, char* groupIp, int port) {
    int socketfd = 0;
    int ret = 0;
    // 检查IP的合法性

    // 创建套接字
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1==socketfd) {
        printf("创建套接字失败, errno %d\n", errno);
        return -TCPIPERR_SOCKET_CREATE;
    }

#if 0
    // 加入组播
    #if 0
    struct in_addr opt;
    // 将组播地址初始化到这个结构体成员中即可
    inet_pton(AF_INET, "224.0.1.0", &opt.s_addr);
    ret = setsockopt(socketfd, IPPROTO_IP, IP_MULTICAST_IF, &opt, sizeof(opt));
    if (-1==ret) {
        printf("加入组播组失败, errno %d\n", errno);
        ret = -TCPIPERR_SOCKET_SETSCOKOPT;
        goto error_set;
    }
    #else
    struct ip_mreq mreq;
    // 设置多播地址
    mreq.imr_multiaddr.s_addr = inet_addr("224.0.1.0");
    // 设置使用该多播地址发送数据时使用的网卡ip
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);	
    // 调用setsockopt
    ret = setsockopt(socketfd, IPPROTO_IP, IP_MULTICAST_IF, &mreq, sizeof(mreq));
    if (-1==ret) {
        printf("加入组播组失败, errno %d\n", errno);
        ret = -TCPIPERR_SOCKET_SETSCOKOPT;
        goto error_set;
    }
    #endif
    #endif

    //  设置发送地址,地址即是组播地址
    struct sockaddr_in cliaddr;
    memset(&cliaddr,0,sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(port); // 接收端需要绑定9999端口
    // 发送组播消息, 需要使用组播地址, 和设置组播属性使用的组播地址一致就可以
    inet_pton(AF_INET, groupIp, &cliaddr.sin_addr.s_addr);

    // 数据广播
    ret = sendto(socketfd, buff, len, 0, (struct sockaddr*)&cliaddr, sizeof(struct sockaddr_in));
    if (-1==ret) {
        printf("发送组播失败, errno %d\n", errno);
        ret = -TCPIPERR_SENDMSG;
    }

error_set:
    close(socketfd);
    return ret;
}

