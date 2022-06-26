#include <stdio.h>

#include "fileXOpt.h"

#define FILE "1.txt"
#define FILE2 "../2.txt"

int main(int argc, char const *argv[])
{
    /* code */
    if (FILEOPT_SUCUSSE!=file_create("1.txt", 666)) {
        printf("创建文件失败\n");
    }

    if (FILECHK_EXIST==file_exist(FILE)) {
        printf("文件存在\n");
    }

    file_rename(FILE, FILE2);

    if(FILEOPT_SUCUSSE!=file_remove(FILE)) {
        printf("删除文件失败\n");
    }

    return 0;
}
