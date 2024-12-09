#include <stdio.h>
#include <unistd.h>
#include "core/task_core.h"

static void test3() {
    printf("\n测试其他任务 1\n");
}

static void test4() {
    printf("\n测试其他任务 2\n");
    //sleep(13);
}

static void test1() {
    printf("\n测试队列任务 1\n");
}

static void test2() {
    printf("\n测试队列任务 2\n");
}

static TASK_ENTRY *entry = NULL;
int main(int argc, char const *argv[])
{
    /* code */
    int ret = 0;
    ret = start_task_core(&entry, 5);
    if (ret<0) {
        printf("启动任务核心失败\n");
    }

#if 1
    while (1) {
        /* code */
#if 1
        commit_task(entry, test1, TASK_TYPE_QUEUE);
        commit_task(entry, test2, TASK_TYPE_QUEUE);
#endif

#if 1
        commit_task(entry, test3, TASK_TYPE_ONECE);
        //sleep(1);
        commit_task(entry, test4, TASK_TYPE_ONECE);
        commit_task(entry, test3, TASK_TYPE_ONECE);
#endif
        printf("一次提交结束\n\n\n\n\n");
        //sleep(3);
        //usleep(10000);
    }
#endif

    sleep(3);
    stop_task_core(entry);

    return 0;
}
