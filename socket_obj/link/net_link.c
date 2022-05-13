/** 
 * 获取指定iface的link状态的模块
*/
#include "net_link.h"
#include "net_errno.h"
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#define IFACE_NUM 2
/* shell 方式获取网络链路状态 */
int get_link_status_shell(const char* ifname) {

}
/** 
 * socket prop 方式获取网络链路状态 
 * return -1，错误 0,link up 1,link down 2,unknow
 */
int get_link_status(const char* ifname) {
    int ret = 0;
    int socketfd = 0;
    int if_num = 0;
    // struct ifreq，保存了网络接口的信息，包括名字、ip、掩码、广播。链路状态、mac地址
    // struct ifconf 理解为ifreq的集合，但是ifreq的内存需要用户指定
    struct ifconf ifc;
    struct ifreq *ifcu_req = NULL;
    struct ifreq ifrs_buf[IFACE_NUM]; // 预计有 IFACE_NUM 个接口
    size_t i = 0;
    // 检查参数
    if (NULL==ifname)
        return -NETERR_CHECK_PARAM;
    // 获取socket，
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd<0)
        return -NETERR_SOCKET_FAIL;
    // 获取ifconf
    ifc.ifc_len = sizeof(ifrs_buf); // 指定获取ifreq的内存大小
    ifc.ifc_ifcu.ifcu_req = ifrs_buf; // 指定获取ifreq放入的内存地址
    if (ioctl(socketfd, SIOCGIFCONF, &ifc)<0) {
        ret = -NETERR_SOCKET_GIFCONF_FAIL;
        goto socket_close_exit;
    }
    // 
    if_num = ifc.ifc_len/sizeof(struct ifreq);
    ifcu_req = ifc.ifc_ifcu.ifcu_req;
    
    // 找到指定名称的网络接口
    for (i = 0; i < if_num; i++) {
        printf("iface name %s \n",ifcu_req[i].ifr_ifrn.ifrn_name);
        if (!strncmp(ifcu_req[i].ifr_ifrn.ifrn_name, ifname, strlen(ifname))) {
            printf("get ifname %s prop \n", ifname);
            break;
        }
    }
    // 如果没有匹配的，指定的接口未找到
    if (i>=if_num) {
        fprintf(stderr, "has no interface:%s \n", ifname);
        ret = -NETERR_HASNO_NETIFNAME;
        goto socket_close_exit;
    }
    ifcu_req +=i;
    //
    if (!(ifcu_req->ifr_ifru.ifru_flags&IFF_UP)) {
        ret = NET_LINK_DOWN; // down
    } else if (!(ifcu_req->ifr_ifru.ifru_flags&IFF_RUNNING)) {
        ret = NET_LINK_UP; // up
    } else {
        ret = NET_LINK_UNKNOW; // unkown
    }
socket_close_exit:
    close(socketfd);
    return ret;
}
int get_iface_ip(const char* ifname) {}
int set_iface_ip(const char* ifname) {}