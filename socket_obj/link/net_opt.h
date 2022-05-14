/**
 * 获取以太网ip，网关，掩码的操作
*/
#ifndef _NET_OPT_H_
#define _NET_OPT_H_

// 获取指定ifname的ip地址
int get_ip(const char* ifname, char *ip, int len);
// 设置指定ifname的ip地址
int set_ip(const char* ifname, const char* ip);
// 获取指定ifname的网关
int get_gateway(const char* ifname, char *ip, int cosnt len);
// 执行shell方式获取网关
int get_gateway_shell(const char* ifname, char* ip, const len);
// 获取指定ifname的掩码
int get_mask(const char* ifname, char *ip, int len);
// 设置指定ifname的掩码
int set_mask(const char* ifname, const char *ip);
// 获取指定ifname的mac地址
int get_mac(const char* ifname, char* mac, int maclen);
// 设置指定ifname的mac地址
int set_mac(const char* ifname, const char* mac);

// 基于libnl获取网络信息
int nl_getGateway();

#endif
