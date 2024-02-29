#include "device_prot.h"
#if !_WIN32
#include "link/net_opt.h"
#include "mac_opt.h"
#endif
#include <string.h>
#include <stdio.h>

int paser_device_info(const char* rsp, DEVICE_BASE_INFO* deviceinfo) {
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

    memset(deviceinfo, 0, sizeof(DEVICE_BASE_INFO));
    if (NULL!=ip)
        memcpy(deviceinfo->ip, ip, strlen(ip));
    if (NULL!=sn)
        memcpy(deviceinfo->sn, sn, strlen(sn));
    return 0;
}

int fill_device_info(DEVICE_BASE_INFO* deviceinfo) {
    if (NULL==deviceinfo)
        return -1;
#if !_WIN32
    // eth ip
    get_ip("eth0", deviceinfo->ip, sizeof(deviceinfo->ip));
    // mac
    int ret = mac_to_snstr(deviceinfo->sn, sizeof(deviceinfo->sn));
    if (ret<0) {
        printf("get mac fail,errno %d\n", ret);
    }
    printf("get mac %s\n", deviceinfo->sn);
#endif
    return 0;
}

int check_base_cmd(DEV_CMD_BASE* bcmd) {
    if (NULL==bcmd)
        return -CMD_RSP_CODE_PARAM;

    // 这几个命令只用到了基础命令,不考虑协议版本差异
     switch (bcmd->cmd) {
        case CMD_DEVICE_SCAN:
        case CMD_SERVER_UP:
        case CMD_DEV_REBOOT:
            return 0;
        break;
        default:
        break;
     }

    if (PROT_VMAJOR!=bcmd->vMajor 
    || PROT_VMINOR!=bcmd->vMinor
    || PROT_VPACK!=bcmd->vtail)
        return -CMD_RSP_CODE_PROT_VER;
    return 0;
}
