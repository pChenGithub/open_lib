#include <stdio.h>
#include "link/net_link.h"

int main(int argc, char* argv[] ) {
    printf("test\n");

    int ret = get_link_status("ens33");
    printf("get link status %d\n", ret);

    return 0;
}