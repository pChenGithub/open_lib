/**
 * http请求接口，
 * 功能：
 * 1、http请求，返回json字符串
 * 2、http请求，返回文件（指定返回文件路径）
 * 3、http请求，上传文件
 * 4、返回请求结果，成功/失败
*/
#ifndef _HTTPC_SVC_H_
#define _HTTPC_SVC_H_

#define HTTPERROR_CHECK_PAREM       1   // 参数检查失败
#define HTTPERROR_OPENFILE          2   // 打开文件失败
#define HTTPERROR_CURL_INIT         3   // curl init失败
#define HTTPERROR_CURL_PERFORM      4   // curl preform失败
#define HTTPERROR_BUFF_NOTENOUGH    5   // 输入缓存不足

typedef void (*http_reply_context_opt)(const char* buffer, int len, void** outstream);
typedef enum {
    RET_NONE,
    RET_FILE,
    RET_JSON,
} httpC_reply_type;

typedef struct {
    httpC_reply_type type;          // 是否有返回值，或者指定返回类型
    http_reply_context_opt opt;
    void* p;                        // 文件描述符或者指向一段内存，如果http请求成功，自己实现的回调需要调用httpC_svc_reply_reset来释放
} httpC_reply;

typedef struct {
    httpC_reply reply;
    int callbk_flag;        // 回调执行成功或者失败，0成功，非0失败
    char* url;
    char* body;
    void* arg;              // 自定义参数，用户自己传入的参数，用户自己管理
} httpC_req_task;

typedef struct {
    char* admin;
    char* pass;
    char* file;
} httpC_UpFileStruct;

// 定义请求回调函数原型类型
typedef void (*http_response_cb)(httpC_req_task* req);
// http请求接口，填入请求结构体和回调函数
int httpC_svc_req(httpC_reply_type type ,const char* url, char* data, const int len);
// int httpC_svc_set_req_cb();
// 释放返回资源
void httpC_svc_reply_reset(httpC_reply* reply);
// 上传日志文件（上传日志专用）
int httpC_uplaodFile(httpC_req_task *req, http_response_cb cb);
#endif



