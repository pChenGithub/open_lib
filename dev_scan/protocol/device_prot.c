#include "device_prot.h"
#if !_WIN32
#include "link/net_opt.h"
#include "mac_opt.h"
#endif
#include <string.h>
#include <stdio.h>

int paser_device_info(const char* rsp, DEVICE_INFO* deviceinfo) {
    const char* ip = NULL;
    const char* sn = NULL;
    if (NULL==rsp || NULL==deviceinfo)
        return -1;

    // IP:172.16.70.185 SN:6ECC0BDF75F0
    char* p = (char*)rsp;
    while (0!=*p) {
        if (0==strncmp(p, "IP:", 3)) {
            ip = p+3;
            if (p>rsp)
                *(p-1) = 0;

            p += 2;
        } else if (0==strncmp(p, "SN:", 3)) {
            sn = p+3;
            if (p>rsp)
                *(p-1) = 0;

            p += 2;
        }
        p++;
    }

    memset(deviceinfo, 0, sizeof(DEVICE_INFO));
    if (NULL!=ip)
        memcpy(deviceinfo->ip, ip, strlen(ip));
    if (NULL!=sn)
        memcpy(deviceinfo->sn, sn, strlen(sn));
    return 0;
}

int fill_device_info(DEVICE_INFO* deviceinfo) {
    if (NULL==deviceinfo)
        return -1;
#if !_WIN32
    // eth ip
    get_ip("eth0", deviceinfo->ip, sizeof(deviceinfo->ip));
    // mac
    int ret = mac_to_snstr(deviceinfo->sn, sizeof(deviceinfo->sn));
    if (ret<0) {
        printf("获取mac失败,错误码 %d\n", ret);
    }
    printf("获取mac %s\n", deviceinfo->sn);
#endif
    return 0;
}
