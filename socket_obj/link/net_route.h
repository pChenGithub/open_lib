/** 
 * 获取网关接口
*/

#ifndef __NET_ROUTE_H__
#define __NET_ROUTE_H__
// 获取指定接口的默认网关
int get_default_gate(const char* ifname, char* gateip, int gateipLen);
#endif