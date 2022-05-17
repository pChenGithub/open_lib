#include <stdio.h>
#include "link/net_dns.h"

int main(int argc, char const *argv[])
{
    int ret = 0;
#if 0
    ret = set_resolv_dns("223.6.6.6");
    if (ret<0) {
        printf("设置dns失败,错误码 %d\n", ret);
    }
#else
    DNS_INFO list[5];
    int len = 5;
    ret = get_resolv_dns(list, &len);
    if (ret<0) {
        printf("获取dns失败,错误码 %d\n", ret);
    }
    printf("获取dns个数 %d\n", len);
    int i =0;
    for (i=0;i<len;i++) {
        printf("dns %s\n", list[i].ip);
    }
#endif    
    return 0;
}
