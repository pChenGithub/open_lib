/**
 * 获取ip，网关，掩码的操作
*/
#ifndef _NET_OPT_H_
#define _NET_OPT_H_

// 获取指定ifname的ip地址
int get_ip(const char* ifname, char *ip, int len);
// 获取指定ifname的网关
int get_gateway(const char* ifname, char *ip, int len);
// 获取指定ifname的掩码
int get_mask(const char* ifname, char *ip, int len);

#endif