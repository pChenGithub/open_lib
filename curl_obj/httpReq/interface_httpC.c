#include "interface_httpC.h"
#include "curl/curl.h"

#include <string.h>
#include <stdlib.h>


#define LOGTAG "httpc_svc"

// 写文件
void http_reply_file(const char* buffer, int len, void** outstream)
{

    FILE* fp = *outstream;
    // 请求返回写入文件
    fwrite(buffer, len, 1, fp);
}

// 写内存
void http_reply_json(const char* buffer, int len, void** outstream)
{
    char* p;
    int lenght = 0;
    if (NULL==*outstream)
    {
        // 分段返回第一段
        // 第一次分配内存
        p = (char*)malloc(len+1);
        if (NULL==p)
        {
            return ;
        }
        // 初始化http请求参数，这里开始指针不为空了
        *outstream = p;
    }
    else
    {
        // 第一段之后返回，修改内存长度
        p = (char*)*outstream;
        lenght = strlen(p);
        p = (char*)realloc(p, lenght+len+1);
        *outstream = p;
        p = p+lenght;
    }

    // 填入返回内容
    memcpy(p, buffer, len);
    p[len] = 0;
}

/**
 * http请求回应
 * 做成可重入的，后面需要采用异步方式
*/
static size_t curl_write_cb(void* buffer, size_t size, size_t nitems, void* arg)
{
    size_t count = size*nitems;
    httpC_req_task* req = (httpC_req_task*)arg;

    //LOG_D("http reply data len %d ", count);
    if (NULL==buffer || 0==count || NULL==req)
    {
        return -1;
    }
    req->reply.opt(buffer, count, &req->reply.p);
    return count;
}

/**
 * 一个http请求,可重入的
 * req http请求结构体
 * data 请求/返回的json字符串（如果返回json），指定文件名（如果获取的文件）
 * len  data长度
 *
*/
int httpC_svc_req(httpC_reply_type type, const char *url, char *data, const int len)
{

    if (NULL==url || NULL==data || len<=0)
        return -HTTPERROR_CHECK_PAREM;

    int ret = 0;
    int lenght = strlen(data);
    httpC_reply reply;
    CURL *curl = NULL;
    CURLcode res;
    char head_pro[64] = {0};

    memset(&reply, 0, sizeof(reply));
    reply.type = type;
    switch (type)
    {
    case RET_FILE:
        reply.opt = http_reply_file;
        // 获取文件句柄
        reply.p = fopen(data, "w");
        if (NULL==reply.p)
        {
            return -HTTPERROR_OPENFILE;
        }
        break;
    case RET_JSON:
        reply.opt = http_reply_json;
        reply.p = NULL;
        break;
    default:
        break;
    }

    snprintf(head_pro, 64, "Content-Length: %d", lenght);
    curl = curl_easy_init();
    if (NULL==curl)
    {
        ret = -HTTPERROR_CURL_INIT;
        goto curlInitError;
    }

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 3000L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "http");
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    headers = curl_slist_append(headers, head_pro);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // body有长度，填入body
    if (RET_JSON==type && 0!=lenght)
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &reply);

    res = curl_easy_perform(curl);
    if (res!=CURLE_OK)
    {
        ret = -HTTPERROR_CURL_PERFORM;
        goto curlInitError;
    }

    //
    if (RET_JSON==type && NULL!=reply.p)
    {
        lenght = strlen((char*)reply.p);
        printf("buff长度 %d  %d \n", len, lenght);
        if (lenght>=len)
        {
            ret = -HTTPERROR_BUFF_NOTENOUGH;
            goto curlInitError;
        }
        memset(data, 0, len);
        memcpy(data, (char*)reply.p, lenght);
        data[lenght] = 0;
    }

curlInitError:
    // 清理资源请求回调产生的资源，返回是文件，需要关掉文件，返回是json，需要free掉
    httpC_svc_reply_reset(&reply);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return ret;
}

/**
 * 复位回应
 * reply 回调中的回应
 *
*/
void httpC_svc_reply_reset(httpC_reply *reply)
{
    switch (reply->type)
    {
    case RET_FILE:
        // http请求返回设置为文件
        if (NULL!=reply->p)
        {   // 文件指针不为空，需要关掉文件，指针置空
            fclose(reply->p);
            reply->p = NULL;
        }
        break;

    case RET_JSON:
        if (NULL!=reply->p)
        {   // http请求返回设置为json
            free(reply->p);     // free掉内存
            reply->p = NULL;
        }
        break;

    default:
        break;
    }
}

// http协议上传文件
int httpC_uplaodFile(httpC_req_task *req, http_response_cb cb)
{
    httpC_UpFileStruct* fileStruct = (httpC_UpFileStruct*)(req->arg);

    int ret = 0;
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(NULL!=curl)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, req->url);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10000L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "http");
        struct curl_slist *headers = NULL;
        //headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_mime *mime;
        curl_mimepart *part;
        mime = curl_mime_init(curl);

        part = curl_mime_addpart(mime);
        curl_mime_name(part, "username");
        curl_mime_data(part, fileStruct->admin, CURL_ZERO_TERMINATED);

        part = curl_mime_addpart(mime);
        curl_mime_name(part, "password");
        curl_mime_data(part, fileStruct->pass, CURL_ZERO_TERMINATED);

        part = curl_mime_addpart(mime);
        curl_mime_name(part, "file");
        curl_mime_filedata(part, fileStruct->file);

        part = curl_mime_addpart(mime);
        curl_mime_name(part, "path");
        curl_mime_data(part, "log", CURL_ZERO_TERMINATED);

        part = curl_mime_addpart(mime);
        curl_mime_name(part, "output");
        curl_mime_data(part, "json", CURL_ZERO_TERMINATED);

        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

        if (NULL!=cb)
        {
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, req);
        }
        switch (req->reply.type)
        {
        case RET_FILE:
            req->reply.opt = http_reply_file;
            break;
        case RET_JSON:
            req->reply.opt = http_reply_json;
            req->reply.p = NULL;
            break;
        default:
            break;
        }

        res = curl_easy_perform(curl);
        if (res!=CURLE_OK)
        {
            ret = -1;
        }
        else if (NULL!=cb)
        {
            cb(req);
            // 如果有用户自定义回调，把自定义回调处理的结果作为结果返回
            // 回调每哟修改 req->callbk_flag 为0
            ret = req->callbk_flag;
        }

        // 清理资源请求回调产生的资源，返回是文件，需要关掉文件，返回是json，需要free掉
        httpC_svc_reply_reset(&req->reply);

        curl_mime_free(mime);
        // 清除头，怀疑是内存泄漏的点
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return ret;
}
