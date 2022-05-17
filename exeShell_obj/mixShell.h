#ifndef __MIXSHELL_H__
#define __MIXSHELL_H__
typedef enum {
    SHELL_RET_OK = 0,
    SHELL_RET_ERR,
    SHELL_RET_IGNORE,
} SHELL_RET_TYPE;
typedef SHELL_RET_TYPE (*shell_ret_line)(const char* linestr);
// C代码混合shell编程
// 执行一个shell命令，并输出命令结果（如果有结果），返回值返回该条命令执行的结果（是否成功/执行状态）
int exeShell(const char* cmd, char* out, const unsigned int out_len);
int exeShellWait(const char* cmd, shell_ret_line handret);
#endif


