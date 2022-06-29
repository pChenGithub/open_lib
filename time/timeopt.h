#ifndef __TIMEOPT_H__
#define __TIMEOPT_H__
#ifdef __cplusplus
extern "C" {
#endif
#define TIMEOPT_ERR_CHECKPARAM      1
#define TIMEOPT_ERR_LOCALTIME       2   // 日历时间格式化本地时间失败
// https://blog.csdn.net/gerryzhu/article/details/50371255  不同的时间和api讲解
// 获取年月日时分秒,数字返回(6个字节)
int getStdTime(char* hour, char* min, char* sec);
int getStdDate(char* year, char* month, char* day);
int getStdDateTime(char dt[6]);
#ifdef __cplusplus
}
#endif
#endif


