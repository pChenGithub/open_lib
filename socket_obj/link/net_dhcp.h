#ifndef __NET_DHCP_H__
#define __NET_DHCP_H__
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    DHCP_RUNNING = 0,
    DHCP_FAIL_END,
    DHCP_SUCC_END,
} DHCP_STATUS;

typedef int (*hand_dhcpstatus)(DHCP_STATUS sta);
int start_dhcp(const char* ifname, int timeouts, hand_dhcpstatus hand);
int stop_dhcp(const char* ifname);
#ifdef __cplusplus
}
#endif
#endif


