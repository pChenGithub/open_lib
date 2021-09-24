#include "mixShell.h"
#include <stdio.h>

#define LOG_E(format, ...) fprintf(stderr, format, ##__VA_ARGS__)
#define LOG_I(format, ...) fprintf(stdout, format, ##__VA_ARGS__)
#define LOG_D(format, ...) fprintf(stdout, format, ##__VA_ARGS__)

int exeShell(const char* cmd, char* out, const unsigned int out_len) {
    // 验证参数
    if (NULL==cmd || NULL==out || out_len==0) {
        LOG_E("参数检查不通过\n");
        return -1;
    }

    // 
    FILE* fp = popen(cmd, "r");
    if (NULL==fp) {
        LOG_E("打开管道出错\n");
        return -1;
    }

    // 读取执行结果
    // 需要命令里面控制获取结果行数，这里默认只取第一行
    if (NULL==fgets(out, out_len-1, fp)) {
        LOG_E("获取命令执行结果失败\n");
        return -1;
    }

    // 执行命令成功
    return 0;
}