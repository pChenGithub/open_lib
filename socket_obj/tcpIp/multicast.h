#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <pthread.h>
#include <sys/types.h>          /* See NOTES */
#if __WIN32
#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

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
} handMulticastArg;
// 回调声明
typedef void(*handMultiaddrMst)(handMulticastArg* arg);
// 组播监听服务实例
typedef struct {
    // 回调函数参数
    handMulticastArg callbackArg;
    // 回调函数
    handMultiaddrMst fn;
    // 线程号
    pthread_t pid;
    // 回复广播消息的发送缓存
    char sendBuff[128];
} RECV_MSG_BODY;
// 启动组播监听, groupIp D类IP地址,port 端口,,监听和发送的groupIp和port要成对
/*
组播需要使用组播地址，在 IPv4 中它的范围从 224.0.0.0 到 239.255.255.255，
并被划分为
局部链接多播地址、
预留多播地址
管理权限多播地址
三类
224.0.0.0 ~ 224.0.0.255: 局部链接多播地址：是为路由协议和其它用途保留的地址，
只能用于局域网中，路由器是不会转发的地址 224.0.0.0 不能用，是保留地址
224.0.1.0 ~ 224.0.1.255: 为用户可用的组播地址（临时组地址），可以用于 Internet 上的。
224.0.2.0 ~ 238.255.255.255: 用户可用的组播地址（临时组地址），全网范围内有效
239.0.0.0 ~ 239.255.255.255: 为本地管理组播地址，仅在特定的本地范围内有效
*/
// **************************************************************
/**
 * @brief 启动组播监听服务,监听指定组播地址和端口的消息,
 * 
 * @param entry             返回RECV_MSG_BODY,要在用完之后调用multicast_listen_del回收
 * @param callback      实现一个自定义的函数处理收到的组播消息,可以传NULL
 * @param groupIp       组播IP地址
 * @param port              组播端口,需要大于10000,或者没有被占用的端口
 * @return * int 
 */
int multicast_listen_start(RECV_MSG_BODY** entry, handMultiaddrMst callback, char* groupIp, int port);
// 销毁组播监听
int multicast_listen_del(RECV_MSG_BODY* entry);
// 回复组播消息
int multicast_resp(RECV_MSG_BODY* entry, char* buff, int len);
//*************************************************************************
// 发送组播消息(字符串len长度包括\0, 数组指定长度)
int multicast_sendmsg(char* buff, int bufflen, int sendsize, char* groupIp, int port, const char* if_ip);
// 广播处理回复消息回调
typedef int(*handMulticastRsp)(char* rspstr, int len);
// 发送组播消息,并超时等到回复
// ms指定为0表示不等带回复
int multicast_sendmsg_wait(char *buff, int bufflen, int sendsize, char *groupIp, int port, const char *if_ip, handMulticastRsp callbk, unsigned int ms);
// ***********************************************************************************
// udp 点对点发送接口
int udp_sendmsg_wait(char* buff, int bufflen, int sendsize, char* ip, int port, handMulticastRsp callbk, unsigned int ms);
#ifdef __cplusplus
}
#endif

