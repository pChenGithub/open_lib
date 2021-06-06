#ifndef _HTTP_DOWNLOAD_FILE_H_
#define _HTTP_DOWNLOAD_FILE_H_

int curl_http_download_file_init();
void curl_http_download_file_deinit();
int curl_http_download_file(const char* url, const char* rename);

#endif

