#include "device_prot.h"
#include <string.h>

int paser_device_info(const char* rsp, DEVICE_INFO* deviceinfo) {
    const char* ip = nullptr;
    const char* sn = nullptr;
    if (nullptr==rsp || nullptr==deviceinfo)
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
    if (nullptr!=ip)
        memcpy(deviceinfo->ip, ip, strlen(ip));
    if (nullptr!=sn)
        memcpy(deviceinfo->sn, sn, strlen(sn));
    return 0;
}
