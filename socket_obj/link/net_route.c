#include "net_route.h"
#include "net_errno.h"
#include <stdio.h>

#define ROUTE_PROC_FILE ""

int get_default_gate(const char* ifname, char* gateip, int gateipLen) {
    int ret = 0;
    char filetext[128] = {0};
    if (NULL==ifname || NULL==gateip || gateipLen<=0)
        return -NETERR_CHECK_PARAM;
    
    FILE* fp = fopen(ROUTE_PROC_FILE, "r");
    if (NULL==fp)
        return -NETERR_FOPEN_FAIL;

    ret = fread(filetext, 128, 1, fp);
    if (ret<=0)
    {
        ret = -NETERR_FREAD_FAIL;
        goto err_exit;
    }

err_exit:
    fclose(fp);
    fp = NULL;
    return ret;
}