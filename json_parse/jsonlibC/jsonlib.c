#include "jsonlib.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int paserItem(cJSON *item, JSON_OBJ_ITEM *retdata);

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

// 解析一个array对象
// obj      数组的json元素
// parent   数组的解析规则
// index    数组元素的序号
static int paserArrayObj(cJSON* obj, JSON_OBJ_ITEM* parent, int index) {
    int size = 0;
    int ret = 0;
    cJSON *item = NULL;
    if (NULL == obj || NULL == parent)
        return -JSONLIB_ERR_PARAM;
    if (NULL==parent->value)
        return -JSONLIB_ERR_PARAM;

    JSON_ARRAY_OBJ_ITEM *retdata = ((JSON_ARRAY_OBJ *)(parent->value))->arrayItem;
    jsonarrayHandOne handFunc = ((JSON_ARRAY_OBJ *)(parent->value))->handFunc;
    char* array = ((JSON_ARRAY_OBJ *)(parent->value))->array;
    int arrayItemSize = ((JSON_ARRAY_OBJ *)(parent->value))->arrayItemSize;
    int arraySize = ((JSON_ARRAY_OBJ *)(parent->value))->arraySize;

    if (NULL==retdata)
        return -JSONLIB_ERR_PARAM;

    // 统计数据变量大小(根据解析规则统计)
    for (int i = 0;;i++) {
        if (NULL==retdata[i].jsonlable)
            break;

        switch (retdata[i].type)
        {
        case TYPE_INT:
            size += 4;
            break;
        case TYPE_FLOAT:
            size += 4;
            break;
        case TYPE_STR:
            size += retdata[i].datalen;
            break;
        case TYPE_IGNORE:
        default:
            // 其他的不统计,这里不考虑数组元素里面有复杂数据
            break;
        }
    }

    if (size<=0) {
        // 没有数据需要解析
        return -JSONLIB_ERR_PASER_IGNORE;
    }

    // 获取数组第index个元素
    char *pbase = array+index*arrayItemSize;
    char *memreq = NULL;
    // 如果没有指定数组进来,则分配内存
    if (NULL==array) {
        pbase = calloc(1, size);
        // 标记有请求内存
        memreq = pbase;
        if (NULL == pbase)
            return -JSONLIB_ERR_MALLOC;
    } else {
        // 如果指定了数组,需要做下面的检查
        if (arrayItemSize!=size) {
            // 解析规则计算的大小和定义数据的大小不匹配
            ret = -JSONLIB_ERR_ARRAY_DATASIZE;
            goto free_exit;
        }

        if (arraySize<(index+1))
        {
            // 超越数组大小
            ret = -JSONLIB_ERR_ARRAY_SIZE;
            goto free_exit;
        }
    }

    char *ptmp = pbase;
    for (int i = 0;; i++)
    {
        if (NULL==retdata[i].jsonlable)
            break;
        // 根据JSON_OBJ_ITEM的jsonlable获取json,解析结果存在value中
        item = cJSON_GetObjectItem(obj, retdata[i].jsonlable);
        // 没有这个字段,不解析
        if (NULL==item)
            continue;

        // 只处理基本类型
        if (TYPE_INT==retdata[i].type) {
            if (cJSON_Number==item->type)
                *(int*)ptmp = item->valueint;
            else if (cJSON_String==item->type)
                *(int*)ptmp = atoi(item->valuestring);
            else {
                ret = -JSONLIB_ERR_FIELDTYPE;
                goto free_exit;
            }
            ptmp += 4;
        } else if (TYPE_FLOAT==retdata[i].type) {
            if (cJSON_Number==item->type)
                *(float *)ptmp = item->valueint;
            else if (cJSON_String==item->type)
                *(float *)ptmp = atof(item->valuestring);
            else {
                ret = -JSONLIB_ERR_FIELDTYPE;
                goto free_exit;
            }
            ptmp += 4;
        } else if (TYPE_STR==retdata[i].type) {
            if (cJSON_String!=item->type) {
                ret = -JSONLIB_ERR_FIELDTYPE;
                goto free_exit;
            }
            jsonlibStrCopy(ptmp, retdata[i].datalen, item->valuestring);
            ptmp += retdata[i].datalen;
        }
        else {
            // 其他的不处理
        }
    }

    // 回调处理数据
    if (NULL!=handFunc)
        ret = handFunc(pbase, size);

free_exit:
    // 释放内存
    if (NULL!=memreq)
        free(memreq);
    
    return ret;
}

// 解析一个json对象
static int paserObj(cJSON* obj, JSON_OBJ_ITEM* retdata) {
    int ret = 0;
    cJSON *item = NULL;
    if (NULL == obj || NULL == retdata)
        return -JSONLIB_ERR_PARAM;

    for (int i = 0;;i++) {
        if (NULL==retdata[i].jsonlable)
            break;
        // 根据JSON_OBJ_ITEM的jsonlable获取json,解析结果存在value中
        item = cJSON_GetObjectItem(obj, retdata[i].jsonlable);
        // 没有这个字段,或者没有指定value,不解析
        if (NULL==item || NULL==retdata[i].value)
            continue;
        ret = paserItem(item, &retdata[i]);
        if (ret<0)
            return ret;
    }
    return 0;
}

// 解析一个json kv
static int paserItem(cJSON* item, JSON_OBJ_ITEM* retdata) {
    cJSON *arrayItem = NULL;
    cJSON *jsonStr = NULL;
    int arraylen = 0;
    int ret = 0;

    if (NULL==item || NULL==retdata)
        return -JSONLIB_ERR_PARAM;

    switch (retdata->type)
    {
    case TYPE_INT:
        if (cJSON_Number==item->type)
            *(int *)(retdata->value) = item->valueint;
        else if (cJSON_String==item->type)
            *(int *)(retdata->value) = atoi(item->valuestring);
        else
            return -JSONLIB_ERR_FIELDTYPE;
        break;
    case TYPE_FLOAT:
        if (cJSON_Number==item->type)
            *(float *)(retdata->value) = item->valueint;
        else if (cJSON_String==item->type)
            *(float *)(retdata->value) = atof(item->valuestring);
        else
            return -JSONLIB_ERR_FIELDTYPE;
        break;
    case TYPE_STR:
        if (cJSON_String!=item->type)
            return -JSONLIB_ERR_FIELDTYPE;
        jsonlibStrCopy(retdata->value, retdata->valuelen, item->valuestring);
        break;
    case TYPE_OBJ:
        if (cJSON_Object!=item->type)
            return -JSONLIB_ERR_FIELDTYPE;
        ret = paserObj(item, retdata->value);
        break;
    case TYPE_ARRAY:
        if (cJSON_Array!=item->type)
            return -JSONLIB_ERR_FIELDTYPE;
        arraylen = cJSON_GetArraySize(item);
        for (int i = 0;i<arraylen;i++) {
            arrayItem = cJSON_GetArrayItem(item, i);
            ret = paserArrayObj(arrayItem, retdata, i);
            if (ret<0)
                break;
        }
        break;
    case TYPE_STR_JSON_OBJ:
        // 认为是一个json,不过需要转一下再处理
        jsonStr = cJSON_Parse(item->valuestring);
        if (NULL==jsonStr)
            return -JSONLIB_ERR_STR2JSON;
        ret = paserObj(jsonStr, retdata->value);
        cJSON_Delete(jsonStr);
        jsonStr = NULL;
        break;
    default:
        return -JSONLIB_ERR_UNKNOW_FIELDTYPE;
        break;
    }
    return ret;
}

int paserJson(const char* jsonstr, JSON_OBJ_ITEM* retdata) {
    int ret = 0;
    if (NULL == jsonstr || NULL == retdata)
        return -JSONLIB_ERR_PARAM;
    // 解析字符串为json
    cJSON *root = cJSON_Parse(jsonstr);
    if (NULL==root)
        return -JSONLIB_ERR_STR2JSON;

    ret = paserObj(root, retdata);
    cJSON_Delete(root);
    root = NULL;

    return ret;
}

/***********************************************************
 * ********************      组包     **********************
 * *********************************************************/
static int fillObj(cJSON *obj, JSON_OBJ_FILL_ITEM *parent);

static int fillArrayItem(cJSON* obj, JSON_OBJ_FILL_ITEM* parent, int index) {
    char tmpstr[12] = {0};
    if (NULL==obj || NULL==parent)
        return -JSONLIB_ERR_PARAM;
    
    // 获取数组元素构成，下面4个变量已经在上一级验证过了
    JSON_ARRAY_OBJ_FILL_ITEM *retdata = ((JSON_ARRAY_OBJ_FILL *)(parent->value))->arrayItem;
    char* array = ((JSON_ARRAY_OBJ *)(parent->value))->array;
    int arrayItemSize = ((JSON_ARRAY_OBJ *)(parent->value))->arrayItemSize;
    int arraySize = ((JSON_ARRAY_OBJ *)(parent->value))->arraySize;

    // 获取数组第index个元素
    char *ptmp = array+index*arrayItemSize;
    // 遍历数组的协议定义
    for (int i=0;;i++) {
        if (NULL==retdata[i].jsonlable)
            break;
        // 数组元素只考虑基本类型了，其他忽略
        switch (retdata[i].type)
        {
        case TYPE_INT:
            if (VTYPE_INT==retdata[i].vType) {
                cJSON_AddNumberToObject(obj, retdata->jsonlable, *(int*)ptmp);
                ptmp += sizeof(int);
            } else if (VTYPE_FLOAT==retdata[i].vType) {
                cJSON_AddNumberToObject(obj, retdata->jsonlable, *(float*)ptmp);
                ptmp += sizeof(float);
            } else if (VTYPE_STR==retdata[i].vType) {
                cJSON_AddNumberToObject(obj, retdata->jsonlable, atoi(ptmp));
                ptmp += retdata[i].datalen;
            }
            else 
                return -JSONLIB_ERR_FIELDTYPE;
            break;
        case TYPE_FLOAT:
            if (VTYPE_INT==retdata[i].vType) {
                cJSON_AddNumberToObject(obj, retdata->jsonlable, *(int*)ptmp);
                ptmp += sizeof(int);
            } else if (VTYPE_FLOAT==retdata[i].vType) {
                cJSON_AddNumberToObject(obj, retdata->jsonlable, *(float*)ptmp);
                ptmp += sizeof(float);
            } else if (VTYPE_STR==retdata[i].vType) {
                cJSON_AddNumberToObject(obj, retdata->jsonlable, atof(ptmp));
                ptmp += retdata[i].datalen;
            }
            else 
                return -JSONLIB_ERR_FIELDTYPE;
            break;
        case TYPE_STR:
            if (VTYPE_INT==retdata[i].vType) {
                snprintf(tmpstr, sizeof(tmpstr), "%d", *(int*)ptmp);
                cJSON_AddStringToObject(obj, retdata->jsonlable, tmpstr);
                ptmp += sizeof(int);
            } else if (VTYPE_FLOAT==retdata[i].vType) {
                snprintf(tmpstr, sizeof(tmpstr), "%f", *(float*)ptmp);
                cJSON_AddStringToObject(obj, retdata->jsonlable, tmpstr);
                ptmp += sizeof(float);
            } else if (VTYPE_STR==retdata[i].vType) {
                cJSON_AddStringToObject(obj, retdata->jsonlable, ptmp);
                ptmp += retdata[i].datalen;
            }
            else 
                return -JSONLIB_ERR_FIELDTYPE;
            break;
        default:
            break;
        }
    }

    
    return 0;
}

static int fillArrayObj(cJSON* objArray, JSON_OBJ_FILL_ITEM* parent) {
    cJSON* item = NULL;
    int ret = 0;
    int size = 0;
    if (NULL==objArray || NULL==parent)
        return -JSONLIB_ERR_PARAM;
    // 获取数组元素构成
    JSON_ARRAY_OBJ_FILL_ITEM *retdata = ((JSON_ARRAY_OBJ_FILL *)(parent->value))->arrayItem;
    // 获取回调
    jsonarrayHandOne handFunc = ((JSON_ARRAY_OBJ *)(parent->value))->handFunc;
    // 获取数组信息
    char* array = ((JSON_ARRAY_OBJ *)(parent->value))->array;
    int arrayItemSize = ((JSON_ARRAY_OBJ *)(parent->value))->arrayItemSize;
    int arraySize = ((JSON_ARRAY_OBJ *)(parent->value))->arraySize;

    if (NULL==retdata)
        return -JSONLIB_ERR_PARAM;

    if (NULL==retdata)
        return -JSONLIB_ERR_PARAM;

    // 统计数据变量大小(根据规则统计)
    for (int i = 0;;i++) {
        if (NULL==retdata[i].jsonlable)
            break;

        switch (retdata[i].vType)
        {
        case VTYPE_INT:
            size += sizeof(int);
            break;
        case VTYPE_FLOAT:
            size += 4;
            break;
        case VTYPE_STR:
            size += retdata[i].datalen;
            break;
        default:
            // 其他的不统计,这里不考虑数组元素里面有复杂数据
            break;
        }
    }

    if (size<=0) {
        // 没有数据需要填充
        return -JSONLIB_ERR_FILL_IGNORE;
    }

    // 数组填充
    if (NULL!=array && arraySize>0) {
        // 如果给的数组大小不对，错误，大小不匹配
        //printf("xxx %d,  %d\n", arrayItemSize, size);
        if (arrayItemSize!=size)
            return -JSONLIB_ERR_ARRAY_DATASIZE;

        for (int i=0;i<arraySize;i++) {
            item = cJSON_CreateObject();
            if (NULL==item)
                return -JSONLIB_ERR_JSON_OBJCREATE;
            // 添加到数组
            cJSON_AddItemToArray(objArray, item);
            // 按规则填充数组元素
            ret = fillArrayItem(item, parent, i);
            if (ret<0)
                return ret;
        }
        return 0;
    }

    //
    if (NULL!=handFunc) {
        // 回调填充
        //ret = handFunc();
        return 0;
    }

    // 既没有数组，也没有回调，参数错误
    return -JSONLIB_ERR_ARRAY_OPT;
}

static int fillItem(cJSON* obj, JSON_OBJ_FILL_ITEM* retdata) {
    int ret = 0;
    char tmpstr[12] = {0};
    cJSON *item = NULL;
    if (NULL == obj || NULL == retdata)
        return -JSONLIB_ERR_PARAM;

    switch (retdata->pType)
    {
    case TYPE_INT:
    case TYPE_FLOAT:
        if (NULL==retdata->value)
            cJSON_AddNumberToObject(obj, retdata->jsonlable, 0);
        else if (VTYPE_INT==retdata->vType)
            cJSON_AddNumberToObject(obj, retdata->jsonlable, *(int*)retdata->value);
        else if (VTYPE_FLOAT==retdata->vType)
            cJSON_AddNumberToObject(obj, retdata->jsonlable, *(float*)retdata->value);
        else if (VTYPE_STR==retdata->vType)
            cJSON_AddNumberToObject(obj, retdata->jsonlable, atoi(retdata->value));
        else
            return -JSONLIB_ERR_FIELDTYPE;
        break;
    case TYPE_STR:
        if (NULL==retdata->value)
            cJSON_AddStringToObject(obj, retdata->jsonlable, "");
        else if (VTYPE_INT==retdata->vType) {
            snprintf(tmpstr, sizeof(tmpstr), "%d", *(int *)retdata->value);
            cJSON_AddStringToObject(obj, retdata->jsonlable, tmpstr);
        }
        else if (VTYPE_FLOAT==retdata->vType) {
            snprintf(tmpstr, sizeof(tmpstr), "%f", *(float *)retdata->value);
            cJSON_AddStringToObject(obj, retdata->jsonlable, tmpstr);
        }
        else if (VTYPE_STR==retdata->vType)
            cJSON_AddStringToObject(obj, retdata->jsonlable, retdata->value);
        else
            return -JSONLIB_ERR_FIELDTYPE;
        break;
        break;
    case TYPE_OBJ:
        // 检查没有传正确参数
        if (NULL==retdata->value)
            return -JSONLIB_ERR_VALUES_NULL;

        item = cJSON_CreateObject();
        if (NULL==item)
            return -JSONLIB_ERR_JSON_OBJCREATE;
        cJSON_AddItemToObject(obj, retdata->jsonlable, item);
        ret = fillObj(item, retdata->value);
        if (ret<0)
            return ret;
        break;
    case TYPE_ARRAY:
        // 检查没有传正确参数
        if (NULL==retdata->value)
            return -JSONLIB_ERR_VALUES_NULL;

        item = cJSON_CreateArray();
        if (NULL==item)
            return -JSONLIB_ERR_JSON_OBJCREATE;

        cJSON_AddItemToObject(obj, retdata->jsonlable, item);
        // 填充json数组
        ret = fillArrayObj(item, retdata);
        if (ret<0)
            return ret;
        break;
    case TYPE_STR_JSON_OBJ:
        break;
    default:
        return -JSONLIB_ERR_UNKNOW_FIELDTYPE;
        break;
    }

    return 0;
}

static int fillObj(cJSON* obj, JSON_OBJ_FILL_ITEM *retdata) {
    int ret = 0;
    if (NULL == obj || NULL == retdata)
        return -JSONLIB_ERR_PARAM;

    for (int i = 0;;i++) {
        // 到达空,结束
        if (NULL==retdata[i].jsonlable)
            break;

        // 忽略字段,忽略
        if (TYPE_IGNORE==retdata[i].pType)
            continue;

        ret = fillItem(obj, &retdata[i]);
        if (ret<0)
            return ret;
    }
    return 0;
}

int fillJson(char *jsonstr, int jslen, JSON_OBJ_FILL_ITEM *retdata)
{
    int ret = 0;
    int len = 0;
    if (NULL == jsonstr || jslen <= 0 || NULL == retdata)
        return -JSONLIB_ERR_PARAM;

    // 先创建一个json
    cJSON* root = cJSON_CreateObject();
    if(NULL == root)
        return -JSONLIB_ERR_STR2JSON;
    // 根据规则拼接json
    ret = fillObj(root, retdata);
    if (ret<0) {
        cJSON_Delete(root);
        root = NULL;
        return ret;
    }

    // 将json转成字符串输出
    char* p = cJSON_PrintUnformatted(root);
    // 销毁回收
    cJSON_Delete(root);
    root = NULL;
    if(NULL == p)
        return -JSONLIB_ERR_JSON2STR;

    //
    len = strlen(p);
    if (jslen <= len) {
        ret = -JSONLIB_ERR_BUFF_NOENOUGH;
        goto free_exit;
    }

    memcpy(jsonstr, p, len+1);

free_exit:
    free(p);
    p = NULL;

    return 0;
}
