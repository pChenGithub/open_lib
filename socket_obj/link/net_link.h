/**
 * 获取网络链接状态
 * */
#ifndef _NET_LINK_H_
#define _NET_LINK_H_
#define NET_LINK_UP         0
#define NET_LINK_DOWN       1
#define NET_LINK_UNKNOW     3
// shell脚本方式获取链接状态
int get_link_status_shell(const char* ifname);
// socket prop 方式获取网络链路状态
int get_link_status(const char* ifname);
/* 获取/设置 iface ip */
int get_iface_ip(const char* ifname);
int set_iface_ip(const char* ifname);
#endif



