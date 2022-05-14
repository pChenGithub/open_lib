#include "mmc_opt.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/statvfs.h>

#define LOG_I(format, ...) printf(format, ##__VA_ARGS__)
#define LOG_E(format, ...) printf(format, ##__VA_ARGS__)

unsigned long long mmc_getsize(const char* dev) {
    unsigned long long size = 0;

    if (NULL==dev) {
        LOG_E("获取块设备大小失败，指定设备路径为空\n");
        return -1;
    }

    int fd = open(dev, O_RDONLY);
    if (fd<0) {
        LOG_E("以只读方式打开设备 %s 失败\n", dev);
        return -1;
    }

    if (ioctl(fd, BLKGETSIZE64, &size)<0) {
        LOG_E("设备 %s 执行 ioctl 失败\n", dev);
        close(fd);
        return -1;
    }

    close(fd);
    return size;
}

int mmc_getFreePercent(const char* path) {
    struct statvfs sta = {0};
    int percent = 0;

    if (NULL==path) {
        LOG_E("获取块设备使用比例失败，指定路径为空\n");
        return -1;
    }

    if (statvfs(path, &sta)<0) {
        LOG_E("获取块设备信息失败\n");
        return -1;
    }

    //LOG_I("分区信息：%ld, %ld, %ld, %ld\n", sta.f_bsize, sta.f_frsize, sta.f_bfree, sta.f_blocks);
    percent = 100*sta.f_bfree/sta.f_blocks;

    return percent;
}


