#include "http_download_file.h"

int main(int argc, char* argv[] ) {

    curl_http_download_file_init();
    /** 涓嬭浇涓€涓?鏂囦欢锛屼繚瀛樺湪鏈?鍦帮紝鍛藉悕涓簍arget.txt */
    curl_http_download_file("https://download.java.net/openjdk/jdk15/ri/openjdk-15+36_linux-x64_bin.tar.gz", "jdk-15.tar.gz");

    curl_http_download_file_deinit();
}