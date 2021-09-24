#include "mixShell.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    char buff[128] = {0};
    /* code */
    exeShell("ifconfig", buff, 128);

    printf("返回结果：%s\n", buff);
    return 0;
}
