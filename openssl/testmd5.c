#include <stdio.h>
#include "Hashopt.h"

int main(int argc, char const *argv[])
{
    /* code */
    const char* str = "1234566666";
    char mdstr[36] = {0};
    Md5StrEncodeToStr(str, mdstr, 36);
    printf("md5str: %s\n", mdstr);
    return 0;
}
