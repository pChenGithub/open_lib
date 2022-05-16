/** 
 * 定义网络接口的错误码
*/
#ifndef __NET_ERRNO_H__
#define __NET_ERRNO_H__
#define NETERR_CHECK_PARAM  1
#define NETERR_FOPEN_FAIL   2   // 执行fopen出错
#define NETERR_FREAD_FAIL   3   // 执行fread出错
#define NETERR_FREAD_ZERO   4   // 执行fread读出0长度内容
#define NETERR_SOCKET_FAIL  5   // 执行socket失败，创建socket失败
#define NETERR_SOCKET_GIFCONF_FAIL      6   // socket获取ifconf失败
#define NETERR_HASNO_NETIFNAME          7   // 没有找到指定的网络接口名称
#define NETERR_SOCKET_GIFADDR_FAIL      8   // socket获取ifaddr(ip地址)失败
#define NETERR_SOCKET_GIFNETMASK_FAIL   9   // socket获取ifnetmask(掩码地址)失败
#define NETERR_SOCKET_SIFADDR_FAIL      10  // socket设置ifaddr(ip地址)失败
#define NETERR_SOCKET_SIFNETMASK_FAIL   11  // socket设置ifnetmask(掩码地址)失败
#define NETERR_BIND_FAIL                12  // 执行bind失败
#define NETERR_PTHREADCREATE_FAIL       13  // 创建线程失败
#define NETERR_NETLINK_ISRUNNING        14  // netlink服务已经开启（重复开启服务线程）
#define NETERR_INVALID_IP               15  // 无效的ip地址
#define NETERR_INVALID_MASK             16  // 无效的掩码
#define NETERR_INVALID_GATEWAY          17  // 无效的网关地址
#define NETERR_INVALID_GATEDIST         18  // 无效的网关目标
#define NETERR_SOCKET_ADDRT             19  // socket设置网关失败
#define NETERR_SOCKET_RTMSG             20  // socket获取网关失败
#endif