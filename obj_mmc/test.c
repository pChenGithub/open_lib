#include <stdio.h>

#include "mmc_opt.h"

#define MMC_DEV "/dev/sda1"

int main(int argc, char const *argv[])
{
    /* code */
    printf("%s 大小 %lld\n", MMC_DEV, mmc_getsize(MMC_DEV));

    printf("%s 空闲比例 %d\n", MMC_DEV, mmc_getFreePercent("."));

    return 0;
}
