#include <stdio.h>
#include <unistd.h>
#include "link/net_netlink.h"

static int hand_ethlink(void* arg)
{
    NETLINK_CB_PARAM* cbarg = (NETLINK_CB_PARAM*)arg;
    printf("%s link状态改变，%d\n", cbarg->ifname, cbarg->link);
}

int main(int argc, char const *argv[])
{
    /* code */
    start_netlink_listen();
    set_netlink_listen_ev(NLEV_LINK, hand_ethlink);

    while (1)
        sleep(100);
    return 0;
}
