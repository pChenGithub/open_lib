
#include "http_download_file.h"

int main(int argc, char* argv[] ) {

    curl_http_download_file_init();
    /** 下载一个文件，保存在本地，命名为target.txt */
    curl_http_download_file("https://image.baidu.com/search/down?tn=download&ipn=dwnl&word=download&ie=utf8&fr=result&url=https%3A%2F%2Fgimg2.baidu.com%2Fimage_search%2Fsrc%3Dhttp%253A%252F%252Fdl.iteye.com%252Fupload%252Fattachment%252F0062%252F1127%252Fa6a8e23b-b993-31eb-9702-71d7aafe9925.jpg%26refer%3Dhttp%253A%252F%252Fdl.iteye.com%26app%3D2002%26size%3Df9999%2C10000%26q%3Da80%26n%3D0%26g%3D0n%26fmt%3Djpeg%3Fsec%3D1625590373%26t%3D2326667fde0b5b70f3973f6d81da6405&thumburl=https%3A%2F%2Fss0.bdstatic.com%2F70cFvHSh_Q1YnxGkpoWK1HF6hhy%2Fit%2Fu%3D119984078%2C1634013610%26fm%3D26%26gp%3D0.jpg", "target.jpg");

    curl_http_download_file_deinit();
}