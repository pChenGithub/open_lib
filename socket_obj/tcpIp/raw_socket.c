#include "raw_socket.h"
#include "tcpIp_errno.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
    unsigned char destmac[6];
    unsigned char srcmac[6];
    unsigned char type[2];  // 协议类型 ipv4(0x0800)/ipv6
} RAW_MAC_HEAD;

typedef struct {
    unsigned char ver_len;                              // 版本号和IP头长度,以4字节为单位
    unsigned char dsf;
    unsigned char toltal_len[2];                    // 总长度,IP头和数据总长度

    unsigned char identification[2];
    unsigned char flags;
    unsigned char frame_offset[2];
    unsigned char ttl;                                          // 跃点数
    unsigned char protocol;                             // 协议类型 udp(17) igmp(2)
    unsigned char head_sum_check[2];    // 头部和校验
    unsigned char src_addr[4];
    unsigned char dest_addr[4];
} RAW_IP_HEAD;

int send_pkg() {
#if __WIN32
    int fd = 0;
#else
    int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
#endif
    if (fd<0)
        return -TCPIPERR_SOCKET_CREATE;

    return 0;
}

#define PRINTF_HEX(buff, len) {\
    for (int i=0;i<len;i++) \
        printf("%02x", buff[i]); \
    printf("\n"); \
}

int recv_pkg(char* recvbuff, int bufflen) {
    int ret = 0;
#if __WIN32
    SOCKET sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (INVALID_SOCKET==sockfd)
#else
    int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sockfd<0)
#endif
        return -TCPIPERR_SOCKET_CREATE;

    struct sockaddr_in srcaddr;
	socklen_t addr_length = sizeof(struct sockaddr_in);

    char ipsrc[16] = {0};
    char destsrc[16] = {0};
    while (1) {
        ret = recvfrom(sockfd, recvbuff, bufflen, 0, (struct sockaddr *)(&srcaddr), &addr_length);
        if (-1==ret) {
            printf("接收消息失败, 错误码 %d\n", errno);
            ret = -TCPIPERR_RECV_DATA;
        }
        printf("\n\n接收消息长度 %d\n", ret);
        RAW_MAC_IP_HEAD* phead = (RAW_MAC_IP_HEAD*)recvbuff;
        printf("目标mac:");
        PRINTF_HEX(phead->destmac, sizeof(phead->destmac));
        printf("源mac:");
        PRINTF_HEX(phead->srcmac, sizeof(phead->srcmac));

#if !__WIN32
        inet_ntop(AF_INET, phead->src_addr, ipsrc, sizeof(ipsrc));
        inet_ntop(AF_INET, phead->dest_addr, destsrc, sizeof(destsrc));
        printf("源地址 %s\n", ipsrc);
        printf("目的地址 %s\n", destsrc);
        sleep(1);
#endif
    }

#if __WIN32
    closesocket(sockfd);
#else
    close(sockfd);
#endif
    return ret;
}

static void* wait_raw_data(void* arg) {
    int ret = 0;
    RAW_MSG_BODY* body = (RAW_MSG_BODY*)arg;
    handRawArg* callArg = &(body->callbackArg);
    // 设置线程名字
    while (1) {
        callArg->len = sizeof(struct sockaddr_in);
        ret = recvfrom(callArg->socketfd, callArg->recvBuff, sizeof(callArg->recvBuff), 0,
                       (struct sockaddr *)(&(callArg->srcaddr)), &(callArg->len));
        if (-1==ret) {
            printf("接收消息失败, 错误码 %d\n", errno);
            continue;
        }

        // 如果设置了回调函数处理
        if (NULL!=body->fn) {
            body->fn(callArg);
        }
    }
    return NULL;
}

int raw_listen_start(RAW_MSG_BODY** entry, int protocalType) {
#if __WIN32
    SOCKET sockfd = 0;
#else
    int sockfd = 0;
#endif
    int ret = 0;
    if (NULL==entry)
        return -TCPIPERR_CHECK_PARAM;

    RAW_MSG_BODY* body = (RAW_MSG_BODY*)calloc(sizeof(RAW_MSG_BODY), 1);
    if (NULL==body)
        return -TCPIPERR_MALLOCA;

#if __WIN32
    WORD sockVersion=MAKEWORD(2,2);
    WSADATA wsaData;//WSADATA结构体变量的地址值
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        printf("WSAStartup() error!");
        free(body);
        return 0;
    }
#endif

    // IPPROTO_ICMP
    sockfd = socket(AF_INET, SOCK_RAW, protocalType);
#if __WIN32
    if (INVALID_SOCKET==sockfd) {
        WSACleanup();
#else
    if (sockfd<0) {
#endif
        ret = -TCPIPERR_SOCKET_CREATE;
        goto free_exit;
    }

    // 接收端,等待接受数据
    ret = pthread_create(&(body->pid), NULL, wait_raw_data, body);
    if (ret<0) {
        ret = -TCPIPERR_PTREAD_CREAT;
        goto close_exit;
    }
    //
    pthread_detach(body->pid);

    // 返回
    *entry = body;
    return 0;

close_exit:
#if __WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
free_exit:
    free(body);
    body = NULL;
    return ret;
}

int raw_listen_stop(RAW_MSG_BODY *entry) {
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
    // 释放
    free(entry);
    return 0;
}

int raw_sendmsg(char *buff, int bufflen, int sendsize, unsigned char destmac[6], unsigned char srcmac[6], int protocalType)
{
    return  raw_sendmsg_wait(buff, bufflen, sendsize, destmac, srcmac, protocalType, NULL, 0);
}

int raw_sendmsg_wait(char *buff, int bufflen, int sendsize, unsigned char destmac[6], unsigned char srcmac[6], int protocalType, handRawRsp callbk, unsigned int ms)
{
    int selret = 0;
#if __WIN32
    SOCKET sockfd = 0;
#else
    int sockfd = 0;
#endif
    int ret = 0;
    if (NULL==buff || bufflen<=0 || sendsize <=0)
        return -TCPIPERR_CHECK_PARAM;

    char* psend = (char*)calloc(sendsize+14, 1);
    if (NULL==psend)
        return -TCPIPERR_MALLOCA;

#if __WIN32
    WORD sockVersion=MAKEWORD(2,2);
    WSADATA wsaData;//WSADATA结构体变量的地址值
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        printf("WSAStartup() error!");
        free(psend);
        return 0;
    }
#endif

    // IPPROTO_ICMP
    sockfd = socket(AF_INET, SOCK_RAW, protocalType);
#if __WIN32
    if (INVALID_SOCKET==sockfd) {
        WSACleanup();
#else
    if (sockfd<0) {
#endif
        ret = -TCPIPERR_SOCKET_CREATE;
        goto free_exit;
    }

    // 填充 psend
    memcpy(psend, srcmac, 6);
    memcpy(psend+6, destmac, 6);
    psend[12] = 0x00;
    psend[13] = 0x08;
    memcpy(psend+14, buff, sendsize);

    struct sockaddr_in cliaddr;
    memset(&cliaddr,0,sizeof(cliaddr));
    // 数据发送
    ret = sendto(sockfd, buff, sendsize, 0, (struct sockaddr*)(&cliaddr), sizeof(cliaddr));
    if (-1==ret) {
#if __WIN32
        printf("send raw fail, errno %d\n", WSAGetLastError());
#else
        printf("send raw fail, errno %d\n", errno);
#endif
        ret = -TCPIPERR_SEND_DATA;
        goto close_exit;
    }

    // 等待数据
    fd_set rfds;
    struct timeval tv;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    // 设置超时时间
    tv.tv_sec = 0;
    tv.tv_usec = 10000;

    // 预计读取数据为超时
    ret = -TCPIPERR_WAITRECV_TIMEOUT;
start_select:
    selret = select(sockfd+1, &rfds, NULL, NULL, &tv);
    if (-1==selret) {
        ret = -TCPIPERR_SELECT;
    } else if (0 == selret) {
        printf("wait data timeout\n");
        //ret = -TCPIPERR_WAITRECV_TIMEOUT;
    } else {
        // >0,有数据
        ret = recvfrom(sockfd, buff, bufflen, 0, NULL, NULL);
        if (-1==ret) {
            ret = -TCPIPERR_RECVMSG;
            goto close_exit;
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

close_exit:
#if __WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
free_exit:
    free(psend);
    psend = NULL;
    return ret;
}
