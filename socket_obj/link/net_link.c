/** 
 * 获取指定iface的link状态的模块
*/
#include "net_link.h"
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#define LOG_E(tag, format, ...) {fprintf(stderr, format, ##__VA_ARGS__);}
#define LOG_I(tag, format, ...) {fprintf(stdout, format, ##__VA_ARGS__);}
#define LOG_D(tag, format, ...) {fprintf(stdout, format, ##__VA_ARGS__);}

#define LOGTAG "get_link"
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
    assert(NULL!=ifname);
    // 获取socket，
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd<0) {
        LOG_E(LOGTAG, "socket create failed\n");
        return -1;
    }
    // 获取ifconf
    ifc.ifc_len = sizeof(ifrs_buf); // 指定获取ifreq的内存大小
    ifc.ifc_ifcu.ifcu_req = ifrs_buf; // 指定获取ifreq放入的内存地址
    if (ioctl(socketfd, SIOCGIFCONF, &ifc)<0) {
        LOG_E(LOGTAG, "ioctl system call failed\n");
        ret = -1;
        goto socket_close_exit;
    }
    //
    if_num = ifc.ifc_len/sizeof(struct ifreq);
    ifcu_req = ifc.ifc_ifcu.ifcu_req;
    LOG_D(LOGTAG, "get iface num %d\n", if_num);
    //
    for (i = 0; i < if_num; i++) {
        LOG_D(LOGTAG, "iface name %s \n",ifcu_req[i].ifr_ifrn.ifrn_name);
        if (!strncmp(ifcu_req[i].ifr_ifrn.ifrn_name, ifname, strlen(ifname))) {
            LOG_I(LOGTAG, "get ifname %s prop \n", ifname);
            break;
        }
    }
    //
    if (i>=if_num) {
        LOG_I(LOGTAG, "has no interface:%s \n", ifname);
        ret = -1;
        goto socket_close_exit;
    }
    ifcu_req +=i;
    //
    if (!(ifcu_req->ifr_ifru.ifru_flags&IFF_UP)) {
        ret = 1; // down
    } else if (!(ifcu_req->ifr_ifru.ifru_flags&IFF_RUNNING)) {
        ret = 0; // up
    } else {
        ret = 2; // unkown
    }
socket_close_exit:
    close(socketfd);
    return ret;
}
int get_iface_ip(const char* ifname) {}
int set_iface_ip(const char* ifname) {}