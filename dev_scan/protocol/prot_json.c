#include "prot_json.h"
#include "../../json_parse/jsonlibC/cJSON.h"
#include <stdio.h>

int fill_sendInfo() {
}

/*
{
    cmdgroup:"",
    cmd:"",
    // getinfo  base
    arg1:"",
}
*/
int parser_getInfo(const char* jsonstr, unsigned char* cmd) {
    if (NULL==jsonstr)
        return -1;

    cJSON* root =  cJSON_Parse(jsonstr);
    if (NULL==root)
        return -2;

    // 获取命令组和命令,参数
    // 返回cmd
    if (NULL!=cmd)
        *cmd = CMD_GET_INFO;

    cJSON_Delete(root);
    root = NULL;
    return 0;
}
