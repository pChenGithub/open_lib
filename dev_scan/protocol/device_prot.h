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
#define CMD_SERVER_UP      9   // 升级服务程序

#define PROT_VMAJOR     0// 大版本
#define PROT_VMINOR     0// 小版本
#define PROT_VPACK      1// 补丁版本版本
typedef struct {
    // 版本
    unsigned int vtail;
    unsigned char vMajor;
    unsigned char vMinor;
    // 命令组
    char cmdgroup;
    // 子命令
    char cmd;
    int code;   // 执行结果
} DEV_CMD_BASE;

typedef struct {
    DEV_CMD_BASE base;
    // 从这里扩展命令,大小控制一下
} DEV_CMD;

#define CMD_RSP_CODE_OK   0   // 命令执行完成
#define CMD_RSP_CODE_REDY   1   // 命令执行完成,准备下一步
// DEV_CMD_RSP 如果修改，至少要大于 DEV_CMD_BASE 的大小 12字节
// 通过cmd的版本判断是否是通版本协议
typedef struct {
    DEV_CMD cmd;
    // 回复信息从这里扩展，大小控制一下
    union {
        DEVICE_INFO dinfo;
    };
} DEV_CMD_RSP;
#define CMD_RSP_INIT(rsp) \
    DEV_CMD_RSP rsp = { \
{{PROT_VPACK,PROT_VMAJOR,PROT_VMINOR,0,0,0},},\
    }

#ifdef __cplusplus
}
#endif
