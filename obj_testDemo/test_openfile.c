#include "stdio.h"

#define LOG_E(format, ...) {fprintf(stderr, format, ##__VA_ARGS__);}
#define LOG_I(format, ...) {fprintf(stdout, format, ##__VA_ARGS__);}
#define LOG_D(format, ...) {fprintf(stdout, format, ##__VA_ARGS__);}

#define FILE_NAME "/data/zytk_reader/img/qss.css"

int main(int argc, char const *argv[]) {
    char buff[1024] = {0};
    /* code */
    FILE* fp = fopen(FILE_NAME, "r");
    if (NULL==fp) {
        LOG_E("打开文件%s失败\n", FILE_NAME);
        return -1;
    }

    // 读文件直到文件尾
    while (EOF!=fread(buff, 1024, 1, fp))
        ;
    
    LOG_I("读文件结束\n");

    return 0;
}
