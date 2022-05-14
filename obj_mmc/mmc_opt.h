#ifndef __MMC_OPT_H__
#define __MMC_OPT_H__
unsigned long long mmc_getsize(const char* dev);
unsigned long long mmc_getUseSize(const char* dev);
// 获取所指定路径所在分区的信息，参数：文件或者文件夹路径
int mmc_getFreePercent(const char* path);
#endif
