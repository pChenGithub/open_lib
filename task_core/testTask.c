#include <stdio.h>
#include <unistd.h>
#include "core/task_core.h"

static TASK_ENTRY *entry = NULL;
int main(int argc, char const *argv[])
{
    /* code */
    int ret = 0;
    ret = start_task_core(&entry, 12);
    if (ret<0) {
        printf("启动任务核心失败\n");
    }

#if 1
    while (1) {
        /* code */
        sleep(100);
    }
#endif

    sleep(3);
    stop_task_core(entry);

    return 0;
}
