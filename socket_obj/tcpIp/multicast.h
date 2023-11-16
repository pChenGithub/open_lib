#ifndef  __MULTICAST_H__
#define __MULTICAST_H__

// 回调参数声明
typedef struct {
    // 接受消息的套接字
    int socketfd;
    // 接收到的广播消息缓存
    char recvBuff[128];
} handMulticastArg;
// 回调声明
typedef void(*handMultiaddrMst)(handMulticastArg* arg);
// 启动组播监听
int multicast_listen(handMultiaddrMst callback);
// 发送组播消息(字符串len长度包括\0, 数组指定长度)
int multicast_sendmsg(char* buff, int len);
#endif