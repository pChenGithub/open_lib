#include "task_core.h"
#include "task_core_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// 处理队列任务实现
static void hand_queue_task() {
    printf("处理队列任务\n");
    while (1)
    {
        printf("处理任务...\n");
        sleep(10);
    }
}

static void* do_thread(void* arg) {
    // 获取线程信息
    THREAD_ENTRY *ppthread = (THREAD_ENTRY*) arg;
    printf("启动线程 %d\n", ppthread->NO);

    while (1)
    {
        sem_wait(&(ppthread->wait_task));
        if (NULL!=ppthread->dofunc)
            ppthread->dofunc();
    }
}
// 停止所有的线程,遍历数组
static void clear_pthreads(TASK_ENTRY* entry) {
    if (NULL==entry)
        return;
    THREAD_ENTRY *ppthread = entry->threads;
    // 对线程数组加锁
    for (int i = 0; i < entry->thread_count;i++) {
        //printf("xxxx %d  %x\n", i,  ppthread[i].status);
        if (!(ppthread[i].status & PTHREAD_STA_RUN))
            continue;
        //printf("停止线程\n");
        pthread_cancel(ppthread[i].pid);
        //printf("回收线程\n");
        pthread_join(ppthread[i].pid, NULL);
        printf("回收完成 %d\n", ppthread[i].NO);

        sem_destroy(&(ppthread[i].wait_task));
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
    // 创建任务线程
    int i = 0;
    for (; i < threadn; i++) {
        ppthread[i].NO = i;
        sem_init(&(ppthread[i].wait_task), 0, 0);
        // 启动线程
        ret = pthread_create(&(ppthread[i].pid), NULL, do_thread, ppthread+i);
        if (0!=ret) {
            ret = -TASK_CORE_THREAD_CREATE;
            goto err_pthread_c;
        }
        ppthread[i].status |= PTHREAD_STA_RUN;
    }
    pptask->thread_count = i ;
    printf("线程数量 %d\n", pptask->thread_count);

    // 初始化链表x2
    ret = init_linkedlist(&(pptask->task_queue));
    if (ret<0) {
        ret = -TASK_CORE_INIT_LINKED;
        goto err_pthread_c;
    }
    ret = init_linkedlist(&(pptask->task_lists));
    if (ret<0) {
        ret = -TASK_CORE_INIT_LINKED;
        goto err_task_lists;
    }

    // 第一个线程用于检查队列任务,初始化,开放
    ppthread[0].dofunc = hand_queue_task;
    sem_post(&(ppthread[0].wait_task));

    *entry = pptask;
    return 0;
err_task_lists:
    free_linkedlist(pptask->task_queue);
err_pthread_c:
    clear_pthreads(pptask);
    free(pptask);
    return ret;
}

void stop_task_core(TASK_ENTRY* entry) {
    if (NULL==entry)
        return ;
    free_linkedlist(entry->task_lists);
    free_linkedlist(entry->task_queue);
    clear_pthreads(entry);
    free(entry);
}
