#include <stdio.h>
#include "link/net_dhcp.h"

static int hand_ret(DHCP_STATUS sta) {
    printf("dhcp状态变化 %d\n", sta);
}

int main(int argc, char const *argv[])
{
    int ret = 0;
    ret = start_dhcp("eth0", 3, hand_ret);
    if (ret<0)
        printf("启动dhcp失败,错误码 %d\n", ret);

    while (1)
    {
        sleep(10);
    }
    
    return 0;
}
