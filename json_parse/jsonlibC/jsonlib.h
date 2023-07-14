#ifndef __JSONLIB_H__
#define __JSONLIB_H__
#ifdef __cplusplus
extern "C" {
#endif
// 错误码
#define JSONLIB_ERR_PARAM               1   // 参数检查失败
#define JSONLIB_ERR_STR2JSON            2   // 字符串转json失败
#define JSONLIB_ERR_FIELDTYPE           3   // 字段类型检查失败
#define JSONLIB_ERR_UNKNOW_FIELDTYPE    4   // 字段类型未知
#define JSONLIB_ERR_PASER_IGNORE        5   // 没有数据需要解析
#define JSONLIB_ERR_MALLOC              6   // 分配内存失败
#define JSONLIB_ERR_ARRAY_DATASIZE      7   // json数组数据大小
#define JSONLIB_ERR_ARRAY_SIZE          8   // json数组长度不足
#define JSONLIB_ERR_JSON2STR            9   // json转字符串失败
#define JSONLIB_ERR_BUFF_NOENOUGH       10  // 缓存不足
#define JSONLIB_ERR_JSON_OBJCREATE      11  // 创建json对象失败
#define JSONLIB_ERR_VALUES_NULL         12  // 传的values字段为空
#define JSONLIB_ERR_ARRAY_OPT           13  // json数组规则错误，没有回调也没有数组
#define JSONLIB_ERR_FILL_IGNORE         14  // 没有数据需要填充

// 协议期望的数据类型
typedef enum
{
    TYPE_OBJ = 0,
    TYPE_ARRAY,
    TYPE_STR_JSON_OBJ,
    // 以下为可以直接返回的类型
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STR,
    // 忽略类型
    TYPE_IGNORE,
} JSON_OBJ_TYPE;

typedef int(*jsonarrayHandOne)(void* paserData, int dataSize);

#define PROP_XXX    0x01

typedef struct {
    const char *jsonlable;  // json标签名称
    JSON_OBJ_TYPE type;     // 期望的输出数据类型
    int datalen;            // 限制参数的长度,字符串和数组需要限制长度,必须指定
    int property;           // 元素属性
} JSON_ARRAY_OBJ_ITEM;

typedef struct {
    JSON_ARRAY_OBJ_ITEM *arrayItem;
    // 处理数组数据,有2种方式,回调处理,或者指定数组接数据
    jsonarrayHandOne handFunc;
    void *array;        // 传入数组的地址(需要解析的数据结构)
    int arrayItemSize;  // 数组元素的大小(字节)
    int arraySize;      // 数组个数
} JSON_ARRAY_OBJ;

typedef struct {
    const char *jsonlable;  // json标签名称
    JSON_OBJ_TYPE type;     // 期望的输出数据类型,期望协议的数据类型
    void *value;            // (第一参数)绑定的数据指针
    int valuelen;           // (第二参数)数据缓存长度,如果是数组或者字符串,这个参数会使用
    int property;           // 元素属性
} JSON_OBJ_ITEM;

/************   填充数据定义   ****************/
// 传入数据的数据类型
typedef enum
{
    VTYPE_INT = 0,
    VTYPE_FLOAT,
    VTYPE_STR,
    VTYPE_ARRAY,
} VALUE_TYPE;

typedef struct {
    const char *jsonlable;  // json标签名称
    JSON_OBJ_TYPE type;     // 协议期望的数据类型
    VALUE_TYPE vType;       // 实际给的的数据类型
    int datalen;            // 限制参数的长度,数组（字符串）需要指定长度,必须指定
    int property;           // 元素属性
} JSON_ARRAY_OBJ_FILL_ITEM;

typedef struct {
    JSON_ARRAY_OBJ_FILL_ITEM *arrayItem;
    // 处理数组数据,有2种方式,回调处理,或者指定数组接数据
    jsonarrayHandOne handFunc;
    void *array;        // 传入数组的地址(需要解析的数据结构)
    int arrayItemSize;  // 数组元素的大小(字节)
    int arraySize;      // 数组个数
} JSON_ARRAY_OBJ_FILL;

typedef struct {
    const char *jsonlable;  // json标签名称
    JSON_OBJ_TYPE pType;    // 协议期望的数据类型
    void *value;            // (第一参数)绑定的数据指针
    VALUE_TYPE vType;       // 实际给的的数据类型
    int valuelen;           // (第二参数)数据缓存长度,如果是数组,这个参数会使用
    int property;           // 元素属性
} JSON_OBJ_FILL_ITEM;

// 解析json字符串
int paserJson(const char* jsonstr, JSON_OBJ_ITEM* retdata);
// json组包
int fillJson(char* jsonstr, int strlen, JSON_OBJ_FILL_ITEM* retdata);
// 解密
int deccode();
#ifdef __cplusplus
}
#endif
#endif
