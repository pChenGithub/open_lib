#include <stdio.h>
#include "jsonlibC/jsonlib.h"

/* 
biz_content={"serialNum":"DE58025CDE44","deviceNum":"202",
"uneventCount":"0","doorCount":"1","doorStatus":"3596",
"softVersion":"5.1.23.0319\n","systemParamVersion":
"1003","terVersion":"946","doorVersion":"1056",
"readerVersion":"896","timeGroupVersion":"926",
"doorHolidayVersion":"30","accbaseInfoVersion":"213321",
"accAuthorityVersion":"213088","cardChangeVersion":"251165",
"recDate":"2023-05-11"}

&charset=utf-8&format=json&method=getParamVersion&sign_type=RSA2&timestamp=2023-05-11 14:17:50&version=1.0&sign=aM4bFXDAaO0bKAO958tVU5glKuY%3D
*/


typedef struct
{
    char serialNum[16];
    int deviceNum;
    int uneventCount;
    int doorCount;
} TEST_DATA;

typedef struct
{
    int eventNum;
    int doorId;
    char cardID[32];
    int enterType;
} TEST_ARRAY_DATA;

TEST_DATA data = {
    "99663322558844",
    11,
    22.1,
    33,
};
TEST_ARRAY_DATA adata;

JSON_OBJ_FILL_ITEM subitems[] = {
    {"eventNum", TYPE_INT},
    {"doorId", TYPE_INT},
    {"cardID", TYPE_STR,},
    {"enterType", TYPE_INT},
    {"readerId", TYPE_INT},
    {"inOrOut", TYPE_INT},
    {"dealTime", TYPE_INT},
    {"deviceSId", TYPE_INT},
    {NULL}
};

JSON_ARRAY_OBJ_ITEM arrayitem[] = {
    {"eventNum", TYPE_INT},
    {"doorId", TYPE_INT},
    {"cardID", TYPE_STR, 32},
};

static int handArray(void* paserData, int dataSize) {
    // 这个检查主要是检查规则(JSON_ARRAY_OBJ_ITEM)
    // 和定义的数据(TEST_ARRAY_DATA)是否匹配
    if (dataSize!=sizeof(TEST_ARRAY_DATA))
        return -JSONLIB_ERR_ARRAY_DATASIZE;
    TEST_ARRAY_DATA *adata = (TEST_ARRAY_DATA *)paserData;

    // 处理TEST_ARRAY_DATA数据,入库或则修改内存
    printf("eventNum %d, doorId %d, cardID %s, enterType %d\n", 
        adata->eventNum, adata->doorId, adata->cardID, adata->enterType);

    return 0;
}

JSON_ARRAY_OBJ arrayRows = {
    arrayitem,
    handArray,
    NULL,
    0,
    0,
};

JSON_OBJ_FILL_ITEM rootdata[] = {
    {"serialNum", TYPE_STR, data.serialNum, VTYPE_STR},
    {"deviceNum", TYPE_STR, &data.deviceNum, VTYPE_INT},
    {"uneventCount", TYPE_INT, &data.uneventCount, VTYPE_INT},
    {"doorCount", TYPE_OBJ, subitems},
    {"doorCount", TYPE_ARRAY, &arrayRows},
    {NULL,}
};

static char jsonstr[1024] = {0};

int main(int argc, char const *argv[])
{
    int ret = 0;
    /* code */
    ret = fillJson(jsonstr, sizeof(jsonstr), rootdata);
    if (ret<0)
        printf("解析json失败,错误码 %d\n", ret);

    printf("jsonstr %s\n", jsonstr);
    return 0;
}

int deccode() {

}
