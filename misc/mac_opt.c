#include "mac_opt.h"
#include "string_opt.h"
#include <stdio.h>
#include <ctype.h>

#define MAC_FILE "/sys/class/net/eth0/address"
//读取mac并转换成sn
int mac_to_snstr(char* buf, int len)
{
    int ret = 0;
    char mac[18] = {0};
    if (NULL==buf || len<13)
        return -MACOPTERR_CHECKPARAM;

    FILE* fp = fopen(MAC_FILE, "r");
    if (NULL==fp)
        return -MACOPTERR_FOPEN_FAIL;

    ret = fread(mac, 1, 18, fp);
    if (18!=ret) {
        ret = -MACOPTERR_FREAD_FAIL;
        goto exit;
    }

    int j = 0;
    int i = 0;
    for (i=0;i<12;)
    {
        buf[i] = toupper(mac[j]);
        buf[i+1] = toupper(mac[j+1]);
        j +=3;
        i +=2;
    }

    buf[len-1] = 0;


exit:
    fclose(fp);
    fp = NULL;
    return ret;
}

int mac_to_snarray(char* buf, int len) {
    int ret = 0;
    printf("xxxxx 1\n");
    char mac[18] = {0};
    if (NULL==buf || len<6)
        return -MACOPTERR_CHECKPARAM;

    ret = mac_to_snstr(mac, 18);
    if (ret<0)
        return ret;

    // 34:14:b5:fc:f6:06
    //
    printf("获取到的mac地址 %s\n", mac);
    if (hexstrToHexchar(buf, len, mac)<0)
        return -MACOPTERR_HEXSTR_TOARRAY;

    return 0;
}
