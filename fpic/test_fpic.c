#include <stdio.h>
#include "share_fuc2.h"

int main() {
    printf("测试fpic\n");
    int a = fuc_add2(1, 2);
    printf("计算结果 fuc_add2(1, 2) = %d\n", a);
    return 0;
}