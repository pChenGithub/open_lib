#include "net_opt.h"
#include "net_errno.h"
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int get_ip(const char *ifname, char* ip, int len)
{
    int ret = 0;
    int socketfd = 0;
    // struct ifreq，保存了网络接口的信息，包括名字、ip、掩码、广播。链路状态、mac地址
    // struct ifconf 理解为ifreq的集合，但是ifreq的内存需要用户指定
    struct ifreq ifcu_req;
    struct sockaddr_in r;
    {
        /* data */
    };
    
    // 检查参数
    if (NULL==ifname || NULL==ip || len<=0)
        return -NETERR_CHECK_PARAM;
    
    // 获取socket，
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd<0)
        return -NETERR_SOCKET_FAIL;

    memcpy(ifcu_req.ifr_ifrn.ifrn_name, ifname, strlen(ifname)+1);
    if (ioctl(socketfd, SIOCGIFADDR, &ifcu_req)<0) {
        ret = -NETERR_SOCKET_GIFADDR_FAIL;
        goto socket_close_exit;
    }

    // 返回ip地址
    struct sockaddr* ip_addr = &ifcu_req.ifr_ifru.ifru_addr;
    snprintf(ip, len, "%u.%u.%u.%u",
            (unsigned char)ip_addr->sa_data[2],
            (unsigned char)ip_addr->sa_data[3],
            (unsigned char)ip_addr->sa_data[4],
            (unsigned char)ip_addr->sa_data[5]);
socket_close_exit:
    close(socketfd);
    return ret;
}

int set_ip(const char* ifname, const char* ip) {
    int ret = 0;
    if (NULL==ifname || NULL==ip)
        return -NETERR_CHECK_PARAM;

    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd<0)
        return -NETERR_SOCKET_FAIL;

    struct ifreq ifcu_req;
    memcpy(ifcu_req.ifr_ifrn.ifrn_name, ifname, strlen(ifname)+1);
    if (ioctl(socketfd, SIOCGIFADDR, &ifcu_req)<0) {
        ret = -NETERR_SOCKET_GIFADDR_FAIL;
        goto socket_close_exit;
    }

    // 修改ip地址
    struct sockaddr_in* addr = (struct sockaddr_in*)&ifcu_req.ifr_ifru.ifru_addr;
    inet_aton(ip, &addr->sin_addr);
#if 0
    // 这里测试过了，这样修改mask函数结束后不能修改调mask
    addr = (struct sockaddr_in*)&ifcu_req.ifr_ifru.ifru_netmask;
    inet_aton(ip, &addr->sin_addr);
#endif
    
    if (ioctl(socketfd, SIOCSIFADDR, &ifcu_req)<0)
        ret = -NETERR_SOCKET_SIFADDR_FAIL;

socket_close_exit:
    close(socketfd);
    return ret;
}

int get_gateway(const char *ifname, char *ip, int len)
{
    int ret = 0;
    return ret;
}

int get_mask(const char *ifname, char *ip, int len)
{
    int ret = 0;
    int socketfd = 0;
    // struct ifreq，保存了网络接口的信息，包括名字、ip、掩码、广播。链路状态、mac地址
    // struct ifconf 理解为ifreq的集合，但是ifreq的内存需要用户指定
    struct ifreq ifcu_req;
    // 检查参数
    if (NULL==ifname || ip==NULL || len<16)
        return -NETERR_CHECK_PARAM;
    // 获取socket，
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd<0)
        return -NETERR_SOCKET_FAIL;

    memcpy(ifcu_req.ifr_ifrn.ifrn_name, ifname, strlen(ifname)+1);
    if (ioctl(socketfd, SIOCGIFNETMASK, &ifcu_req)<0) {
        ret = -NETERR_SOCKET_GIFNETMASK_FAIL;
        goto socket_close_exit;
    }

    // 返回ip地址
    struct sockaddr* ip_addr = &ifcu_req.ifr_ifru.ifru_netmask;
    snprintf(ip, len, "%u.%u.%u.%u",
            (unsigned char)ip_addr->sa_data[2],
            (unsigned char)ip_addr->sa_data[3],
            (unsigned char)ip_addr->sa_data[4],
            (unsigned char)ip_addr->sa_data[5]);
socket_close_exit:
    close(socketfd);
    return ret;
}

int set_mask(const char* ifname, const char *ip) {
    int ret = 0;
    if (NULL==ifname || NULL==ip)
        return -NETERR_CHECK_PARAM;

    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd<0)
        return -NETERR_SOCKET_FAIL;

    struct ifreq ifcu_req;
    memcpy(ifcu_req.ifr_ifrn.ifrn_name, ifname, strlen(ifname)+1);
    if (ioctl(socketfd, SIOCGIFNETMASK, &ifcu_req)<0) {
        ret = -NETERR_SOCKET_GIFNETMASK_FAIL;
        goto socket_close_exit;
    }

    // 修改ip地址
    struct sockaddr_in* addr = (struct sockaddr_in*)&ifcu_req.ifr_ifru.ifru_netmask;
    inet_aton(ip, &addr->sin_addr);
 
    if (ioctl(socketfd, SIOCSIFNETMASK, &ifcu_req)<0)
        ret = -NETERR_SOCKET_SIFNETMASK_FAIL;

socket_close_exit:
    close(socketfd);
    return ret;
}


int nl_getGateway()
{

}


