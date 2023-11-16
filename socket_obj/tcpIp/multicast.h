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
int multicast_listen(RECV_MSG_BODY** entry, handMultiaddrMst callback, char* groupIp, int port);
// 销毁组播监听
int multicast_listen_del(RECV_MSG_BODY* entry);
// 发送组播消息(字符串len长度包括\0, 数组指定长度)
int multicast_sendmsg(char* buff, int len, char* groupIp, int port);
#endif