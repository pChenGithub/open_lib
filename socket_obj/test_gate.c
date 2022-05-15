#include <stdio.h>
#include "link/net_opt.h"

int main(int argc, char const *argv[])
{
    int ret = 0;
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
    return 0;
}
