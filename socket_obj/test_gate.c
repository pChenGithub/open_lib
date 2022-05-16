#include <stdio.h>
#include "link/net_opt.h"
#include "link/net_netlink.h"

int main(int argc, char const *argv[])
{
    int ret = 0;
#if 0
    // 设置网关
    //ret = set_gateway("ens33", "192.168.0.3", "255.255.0.0", "192.168.1.1");
    // 添加默认网关
    //ret = set_gateway("ens33", "192.168.0.1", NULL, NULL);
    // 添加指定目标网关，
    ret = set_gateway("ens33", "192.168.0.13", "255.255.255.255", "192.168.0.13");
    if (ret<0)
    {
        printf("设置网关失败，错误码%d\n", ret);
    }
#else
    ROUTE_LIST list[2] = {0};
    int len = 2;
    //ret = nl_getGateways("default", NULL, list, &len);
    ret = nl_getGateways(NULL, NULL, list, &len);
    //ret = get_gateway(NULL, NULL, 0);
    if (ret<0)
        printf("获取网关失败，错误码%d\n", ret);

    int i = 0;
    printf("获取个数 %d\n", len);
    for (i=0;i<len;i++) {
        printf("%s %s %s\n", list[i].dist, list[i].gate, list[i].devname);
    }
#endif
    return 0;
}
