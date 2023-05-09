#ifndef __JSONLIB_H__
#define __JSONLIB_H__
#ifdef __cplusplus
extern "C" {
#endif
// 错误码
#define JSONLIB_ERR_PARAM       1   // 参数检查失败
#define JSONLIB_ERR_STR2JSON    2   // 字符串转json失败

typedef enum
{
    TYPE_OBJ = 0,
    TYPE_ARRAY,
    TYPE_STR_JSON,
    // 以下为可以直接返回的类型
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STR,
} JSON_OBJ_TYPE;

typedef struct
{
    const char *jsonlable;
    JSON_OBJ_TYPE type;
    void *value;
    int valuelen;
} JSON_OBJ_ITEM;

// 解析json字符串
int paserJson(const char* jsonstr, JSON_OBJ_ITEM* retdata, int len);
// json组包
int fillJson(char* jsonstr, int strlen, JSON_OBJ_ITEM* retdata);
#ifdef __cplusplus
}
#endif
#endif
