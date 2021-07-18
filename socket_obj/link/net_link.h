#ifndef _NET_LINK_H_
#define _NET_LINK_H_
int get_link_status_shell(const char* ifname);
/* socket prop 方式获取网络链路状态 */
int get_link_status(const char* ifname);
/* 获取/设置 iface ip */
int get_iface_ip(const char* ifname);
int set_iface_ip(const char* ifname);
#endif



