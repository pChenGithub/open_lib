#include <stdio.h>
#include "link/net_link.h"
#include "link/net_opt.h"

#define IFNAME "enp0s3"

int main(int argc, char* argv[] ) {
    char ip[16] = {0};
    printf("test\n");

    int ret = get_link_status(IFNAME);
    printf("get link status %d\n", ret);

    ret = set_ip(IFNAME, "172.16.70.183");
    if (ret<0)
    {
        printf("set %s ip fail, ret %d\n", IFNAME, ret);
    }

    ret = get_ip(IFNAME, ip, 16);
    if (ret<0)
    {
        printf("get %s ip fail, ret %d\n", IFNAME, ret);
        return -1;
    }
    printf("get %s ip ok, ip %s\n", IFNAME, ip);

    ret = get_mask(IFNAME, ip, 16);
    if (ret<0)
    {
        printf("get %s mask fail, ret %d\n", IFNAME, ret);
        return -1;
    }
    printf("get %s mask ok, ip %s\n", IFNAME, ip);



    return 0;
}