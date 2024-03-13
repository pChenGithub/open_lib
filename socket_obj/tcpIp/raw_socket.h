#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <pthread.h>
#if __WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ether.h>		//ETH_P_ALL
#endif

typedef struct {
    // MAC头
    unsigned char destmac[6];
    unsigned char srcmac[6];
    unsigned char type[2];  // 协议类型 ipv4(0x0800)/ipv6
    // IP头
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
} RAW_MAC_IP_HEAD;

int send_pkg();
int recv_pkg(char* recvbuff, int bufflen);
// **************************************************************
// 回调参数声明
typedef struct {
    // 接受消息的套接字
#if __WIN32
    SOCKET socketfd;
#else
    int socketfd;
#endif
    // 消息源地址
    struct sockaddr_in srcaddr;
    socklen_t  len;
    // 接收到的广播消息缓存
    char recvBuff[128];
} handRawArg;
// 回调声明
typedef void(*handRawMsg)(handRawArg* arg);
// raw监听服务实例
typedef struct {
    // 回调函数参数
    handRawArg callbackArg;
    // 回调函数
    handRawMsg fn;
    // 线程号
    pthread_t pid;
} RAW_MSG_BODY;
// 启用mac层数据监听
int raw_listen_start(RAW_MSG_BODY **entry, int protocalType);
int raw_listen_stop(RAW_MSG_BODY *entry);
// 发送原始套接字消息
int raw_sendmsg(char* buff, int bufflen, int sendsize, unsigned char destmac[6], unsigned char srcmac[6], int protocalType);
// 处理回复消息回调
typedef int(*handRawRsp)(char* rspstr, int len);
// 发送消息,并超时等到回复
// ms指定为0表示不等带回复
int raw_sendmsg_wait(char *buff, int bufflen, int sendsize, unsigned char destmac[], unsigned char srcmac[], int protocalType, handRawRsp callbk, unsigned int ms);

#ifdef __cplusplus
}
#endif

