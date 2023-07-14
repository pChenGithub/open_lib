#include <stdio.h>
#include "jsonlibC/jsonlib.h"

const char *jsonstr = "{\"serialNum\":\"DE58025CDE44\",\"deviceNum\":\"202\",\"count\":\"1\",\"rows\":[{\"eventNum\":\"35\",\"doorId\":\"1\",\"cardID\":\"2598580480\",\"enterType\":\"2\",\"readerId\":\"1\",\"inOrOut\":\"1\",\"dealTime\":\"2023-05-09 15:37:57\",\"deviceSId\":\"52\"},{\"eventNum\":\"36\",\"doorId\":\"7\",\"cardID\":\"2598580497\",\"enterType\":\"2\",\"readerId\":\"1\",\"inOrOut\":\"1\",\"dealTime\":\"2023-05-09 15:37:57\",\"deviceSId\":\"52\"}]}";


typedef struct
{
    int deviceNum;
    int count;
} TEST_DATA;


typedef struct
{
    int eventNum;
    int doorId;
    char cardID[32];
    int enterType;
} TEST_ARRAY_DATA;

TEST_DATA data;

#define ARRAY_SIZE  2
TEST_ARRAY_DATA adata[ARRAY_SIZE];

JSON_ARRAY_OBJ_ITEM array_item[] = {
    {"eventNum", TYPE_INT},
    {"doorId", TYPE_INT},
    {"cardID", TYPE_STR, 32},
    {"enterType", TYPE_INT},
    {"readerId", TYPE_IGNORE},
    {"inOrOut", TYPE_IGNORE},
    {"dealTime", TYPE_IGNORE},
    {"deviceSId", TYPE_IGNORE},
    {NULL}
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
    array_item,
    handArray,
    adata,
    sizeof(TEST_ARRAY_DATA),
    sizeof(adata)/sizeof(TEST_ARRAY_DATA),
};

JSON_OBJ_ITEM rootdata[] = {
    {"serialNum", TYPE_STR, NULL},
    {"deviceNum", TYPE_INT, &data.deviceNum},
    {"count", TYPE_INT, &data.count},
    {"rows", TYPE_ARRAY, &arrayRows},
    {NULL,}
};

int main(int argc, char const *argv[])
{
    int ret = 0;
    /* code */
    ret = paserJson(jsonstr, rootdata);
    if (ret<0)
        printf("解析json失败,错误码 %d\n", ret);

    printf("deviceNum %d, count %d\n", data.deviceNum, data.count);
    for (int i=0;i<ARRAY_SIZE;i++) {
        printf("eventNum %d, doorId %d, cardID %s\n", adata[i].eventNum, adata[i].doorId, adata[i].cardID);
        printf("enterType %d\n", adata[i].enterType);
    }
    return 0;
}

int deccode() {

}
