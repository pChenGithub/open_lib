#include "http_download_file.h"
#include "curl/curl.h"

/** 下载文件的curl handle*/
static CURL* curl_handle = NULL;

/* 下载内容操作回调 */
static size_t curl_write_cb(void* buffer, size_t size, size_t nitems, void* outstream) {
    size_t count = size*nitems;
    FILE* fp = (FILE*)outstream;

    if (NULL==buffer || 0==count || NULL==outstream)
        return -1;
    
    printf("write to file %ld \n", count);
    count = fwrite(buffer, size, nitems, fp);
    //fclose(fp);
    printf("write to file count %ld \n", count);

    return count;

}

/** 
 * 下载进度回调 
 * dltotal 下载
 * ultotal 上传
 */
static int curl_progress_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
#if 0
    printf("download dltotal %lf \n", dltotal);
    printf("download dlnow %lf \n", dlnow);
    printf("download ultotal %lf \n", ultotal);
    printf("download ulnow %lf \n\n\n\n\n", ulnow);
#endif
    return 0;
}

int curl_http_download_file_init() {
    CURLcode error;

    curl_global_init(CURL_GLOBAL_SSL);
    curl_handle = curl_easy_init();

    /* 给curl设置属性 */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_write_cb);
    //curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 500);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);

    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(curl_handle, CURLOPT_PROGRESSFUNCTION, curl_progress_cb);
	curl_easy_setopt(curl_handle, CURLOPT_PROGRESSDATA, NULL);
    

    return 0;
}

void curl_http_download_file_deinit() {
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
}

int curl_http_download_file(const char* url, const char* rename) {
    CURLcode error;
    FILE* fp = fopen(rename, "wb");

    if (NULL==fp) {
        printf("open file error \n");
        return -1;
    }

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);

    error = curl_easy_perform(curl_handle); // 会阻塞吗？？？，阻塞的
    if (error!=CURLE_OK) {
        printf("curl_easy_perform curl_handle error, errno %d \n", error);
        fclose(fp);
        return error;
    }

    printf("curl_easy_perform success \n");
    fclose(fp);
    return 0;
}




