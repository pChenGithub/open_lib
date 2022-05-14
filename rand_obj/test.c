#include <stdio.h>

#include "rand_ops.h"


int main(int argc, char const *argv[])
{
    /* code */
    printf("获取随机数 %d\n", rand_get(1, 1));
    
    printf("获取随机数 %d\n", rand_get_0to3600(1, 2));
    return 0;
}
