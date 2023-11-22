#ifndef __TCPIP_ERRNO_H__
#define __TCPIP_ERRNO_H__

#define TCPIPERR_CHECK_PARAM  1
#define TCPIPERR_SOCKET_CREATE  2   // 创建套接字失败
#define TCPIPERR_SOCKET_BIND  3         // 套接字绑定IP失败
#define TCPIPERR_SOCKET_SETSCOKOPT 4     // 套接属性设置失败
#define TCPIPERR_PTREAD_CREAT 5     // 创建线程失败
#define TCPIPERR_MALLOCA 6     // 分配内存失败
#define TCPIPERR_SENDMSG 7     // 发送组播失败
#define TCPIPERR_RECVMSG 8     // 接收消息失败
#define TCPIPERR_WAITRECV_TIMEOUT  9     // 等待数据超时
#define TCPIPERR_SELECT  10     // select失败


#endif

