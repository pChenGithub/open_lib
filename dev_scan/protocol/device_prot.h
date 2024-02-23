#pragma once
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char sn[16];
    char ip[20];
} DEVICE_INFO;
int paser_device_info(const char* rsp, DEVICE_INFO* deviceinfo);
int fill_device_info(DEVICE_INFO* deviceinfo);

// 命令协议
#define CMD_DEVICE_SCAN     1   // 扫描
#define CMD_DEVICE_UPDATE   2   // 升级
#define CMD_GET_LOGS        3   // 获取日志
#define CMD_GET_PKG         4   // 获取安装包
#define CMD_DO_SHELL        5   // 执行脚本
#define CMD_DEV_REBOOT      6   // 设备重启
#define CMD_DEV_INIT      7   // 设备初始化
#define CMD_DEV_REGAUID      8   // 设备恢复出厂
typedef struct {
    char cmdgroup;
    char cmd;
    char arg1;
    char arg2;
} DEV_CMD;

#define CMD_RSP_CODE_OK   0   // 命令执行完成
#define CMD_RSP_CODE_REDY   1   // 命令执行完成,准备下一步
typedef struct {
    DEV_CMD cmd;
    int code;   // 执行结果
} DEV_CMD_RSP;



#ifdef __cplusplus
}
#endif
