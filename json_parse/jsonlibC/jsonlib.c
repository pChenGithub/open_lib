#include "jsonlib.h"
#include "cJSON.h"
#include <stdio.h>

static int jsonlibStrCopy(char* buff, int len, const char* str) {
    if (NULL==buff || len<=0 || NULL==str)
        return -JSONLIB_ERR_PARAM;
    int slen = strlen(str);
    if (slen>=len)
        slen = len - 1;
    memcpy(buff, str, slen);
    buff[slen] = 0;
    return 0;
}

static int paserItem(cJSON* obj, JSON_OBJ_ITEM* retdata, int len) {
    cJSON *item = NULL;
    if (NULL == obj || NULL == retdata || len <= 0)
        return -JSONLIB_ERR_PARAM;

    for (int i = 0;i<len;i++) {
        item = cJSON_GetObjectItem(obj, retdata[i].jsonlable);
        if (NULL==item)
            continue;
        switch (retdata[i].type)
        {
        case TYPE_INT:
            *(int *)(retdata[i].value) = item->valueint;
            break;
        case TYPE_FLOAT:
            *(float *)(retdata[i].value) = item->valuedouble;
            break;
        case TYPE_STR:
            jsonlibStrCopy(retdata[i].value, retdata[i].valuelen, item->valuestring);
            break;
        case TYPE_OBJ:
            paserItem(item, retdata[i].value, retdata[i].valuelen);
            break;
        case TYPE_ARRAY:
            break;
        default:
            break;
        }
    }
}

int paserJson(const char* jsonstr, JSON_OBJ_ITEM* retdata, int len) {
    int ret = 0;
    if (NULL == jsonstr || NULL == retdata)
        return -JSONLIB_ERR_PARAM;
    // 解析字符串为json
    cJSON *root = cJSON_Parse(jsonstr);
    if (NULL==root)
        return -JSONLIB_ERR_STR2JSON;

    ret = paserItem(retdata, len);
    cJSON_Delete(root);
    root = NULL;

    return ret;
}