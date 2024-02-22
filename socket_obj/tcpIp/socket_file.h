#pragma once
#include "tcpIp_errno.h"
#if __WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <pthread.h>
#include <netinet/in.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#endif

// 发送和接收文件
int sock_send_file(int socketfd, const char* file);
int sock_recv_file(int socketfd, const char* file);
// 发送和接收

//********************************************************************************
// tcp通信
// 回调参数声明
typedef struct {
    // 接受消息的套接字
    int socketfd;
    // 消息源地址
    struct sockaddr_in srcaddr;
    socklen_t  len;
    // 接收到的广播消息缓存
    char recvBuff[128];
} handTcpArg;
// 回调
typedef void(*handTcpMsg)(handTcpArg* arg);
// tcp监听服务实例
typedef struct {
    // 回调函数参数
    handTcpArg callbackArg;
    // 回调函数
    handTcpMsg fn;
    // 线程号
    pthread_t pid;
    // 回复广播消息的发送缓存
    char sendBuff[128];
} RECV_TCP_MSG_BODY;
// 启动tcp监听服务
int tcp_listen_start(RECV_TCP_MSG_BODY** entry, handTcpMsg callback, char* serverip, int port);
// 销毁tcp监听
int tcp_listen_del(RECV_TCP_MSG_BODY* entry);

