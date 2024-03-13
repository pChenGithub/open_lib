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
#define TCPIPERR_OPENFILE   11  // 打开文件失败
#define TCPIPERR_FSTAT   12  // 获取文件状态
#define TCPIPERR_SEND_DATA   13  // 发送数据失败
#define TCPIPERR_RECV_DATA   14  // 接收数据失败
#define TCPIPERR_RSP   15  // 回复数据异常
#define TCPIPERR_RSP_INDEX   16  // 回复数据序号异常

#define TCPIPERR_RSP_CODE   17  // 回复数据返回值异常

#define TCPIPERR_READ_FILE   18  // 读取文件失败
#define TCPIPERR_WRITE_FILE   19  // 读取文件失败

#define TCPIPERR_SOCKET_LISTEN   20  // 监听socket失败
#define TCPIPERR_CONNECT 21     // socket连接失败

#define TCPIPERR_CALLBACK 22     // 回调函数返回异常

#define TCPIPERR_BUFF_SIZE  23  // 缓存大小异常

#endif

