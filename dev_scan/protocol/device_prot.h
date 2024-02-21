#pragma once
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char sn[16];
    char ip[20];
} DEVICE_INFO;
int paser_device_info(const char* rsp, DEVICE_INFO* deviceinfo);

// 命令协议
#define CMD_DEVICE_SCAN     1   // 扫描
#define CMD_DEVICE_UPDATE   2   // 升级
#define CMD_GET_LOGS        3   // 获取日志
#define CMD_GET_PKG         4   // 获取安装包
#define CMD_DO_SHELL        5   // 执行脚本
typedef struct {
    char cmdgroup;
    char cmd;
    char arg1;
    char arg2;
} DEV_CMD;

typedef struct {
    DEV_CMD cmd;
    int code;   // 执行结果
} DEV_CMD_RSP;



#ifdef __cplusplus
}
#endif
