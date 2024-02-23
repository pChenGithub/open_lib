#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "tcpIp_errno.h"
#include <pthread.h>
#if __WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <netinet/in.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#endif

// 发送和接收文件
#if __WIN32
int sock_send_file(SOCKET socketfd, const char* file);
int sock_recv_file(SOCKET socketfd, const char* file);
#else
int sock_send_file(int socketfd, const char* file);
int sock_recv_file(int socketfd, const char* file);
#endif
// tcp打开socket，绑定地址和端口
// 这两个接口都是客户端
int ip_send_file(const char* ip, int port, const char* file);
int ip_recv_file(const char* ip, int port, const char* file);

// 发送和接收

//********************************************************************************
// tcp通信
// 回调参数声明
typedef struct {
    // 接受消息的套接字
#if __WIN32
    SOCKET msgfd;
#else
    int msgfd;
#endif
    // 消息源地址
    struct sockaddr_in srcaddr;
    socklen_t  len;
    // 接收到的消息缓存
    //char recvBuff[128];
} handTcpArg;
// 回调
typedef int(*handTcpMsg)(handTcpArg* arg);
// tcp监听服务实例
typedef struct {
    // 回调函数参数
    handTcpArg callbackArg;
    #if __WIN32
    SOCKET socketfd;
#else
    int socketfd;
#endif
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

#ifdef __cplusplus
}
#endif


