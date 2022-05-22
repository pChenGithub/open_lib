#include <stdio.h>
#include <stdlib.h>
#include "link/net_opt.h"
int main(int argc, char const *argv[])
{
    int ret = 0;
#if 0
    ret = get_link("eth0");
    if (ret<0) {
        printf("获取网络状态失败，错误码 %d\n", ret);
    }
    printf("网络状态 %d\n", ret);
#else
    int opt = atoi(argv[1]);
    ret = set_enable("eth0", opt);
    if (ret<0) {
        printf("设置网络状态失败，错误码 %d\n", ret);
    }
#endif
    return 0;
}
