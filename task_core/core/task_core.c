#include "task_core.h"
#include "task_core_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void* do_thread(void* arg) {
}
// 停止所有的线程,遍历数组
static void clear_pthreads(TASK_ENTRY* entry) {
    if (NULL==entry)
        return;
    THREAD_ENTRY *ppthread = entry->threads;
    // 对线程数组加锁
    for (int i = 0; i < entry->thread_count;i++) {
        if (!(ppthread[i].status&PTHREAD_STA_RUN))
            continue;
        printf("停止线程\n");
        pthread_cancel(ppthread[i].pid);
        printf("回收线程\n");
        pthread_join(ppthread[i].pid, NULL);
        printf("回收完成\n");
        memset(ppthread + i, 0, sizeof(THREAD_ENTRY));
    }
    // 解锁
}

int start_task_core(TASK_ENTRY **entry, unsigned int threadn)
{
    int ret = 0;
    TASK_ENTRY *pptask = NULL;
    if (NULL == entry || 0 == threadn)
        return -TASK_CORE_CHECK_PARAM;

    pptask = (TASK_ENTRY*)calloc(1, sizeof(TASK_ENTRY));
    if (NULL==pptask)
        return -TASK_CORE_MALLOC;

    if (threadn>MAX_THREAD_COUNT)
        threadn = MAX_THREAD_COUNT;

    THREAD_ENTRY *ppthread = pptask->threads;
    // 创建线程
    int i = 0;
    for (; i < threadn; i++) {
        ret = pthread_create(&(ppthread[i].pid), NULL, do_thread, ppthread+i);
        if (0!=ret) {
            ret = -TASK_CORE_THREAD_CREATE;
            goto err_pthread_c;
        }
        ppthread[i].status |= PTHREAD_STA_RUN;
    }
    pptask->thread_count = i + 1;

    // 初始化链表x2

    // 第一个线程用于检查队列任务

    return 0;
err_pthread_c:
    clear_pthreads(pptask);
    free(pptask);
    return ret;
}

void stop_task_core(TASK_ENTRY* entry) {
    if (NULL==entry)
        return ;
    clear_pthreads(entry);
    free(entry);
}
