#include "timeopt.h"
#include <time.h>

int getStdTime(char* hour, char* min, char* sec) {
    return 0;
}

int getStdDate(char* year, char* month, char* day) {
    return 0;
}

// 从 0-5,表示 年月日时分秒
int getStdDateTime(char dt[6]) {
    if (NULL==dt)
        return -TIMEOPT_ERR_CHECKPARAM;

    struct tm tmptm;
    time_t tnow = time(NULL);
    // 对比localtime,localtime_r是线程安全的
    if (NULL==localtime_r(&tnow, &tmptm))
        return -TIMEOPT_ERR_LOCALTIME;

    dt[0] = tmptm.tm_year % 100;
    dt[1] = tmptm.tm_mon+1;
    dt[2] = tmptm.tm_mday;

    dt[3] = tmptm.tm_hour;
    dt[4] = tmptm.tm_min;
    dt[5] = tmptm.tm_sec;

    return 0;
}