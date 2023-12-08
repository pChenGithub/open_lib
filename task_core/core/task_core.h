#pragma onece
#include "../../dstruct/linkedlist/linkedlist.h"
#include <pthread.h>

typedef struct {
    // 
    pthread_t pid;
    unsigned int status;
} THREAD_ENTRY;
#define MAX_THREAD_COUNT    10
// 线程状态的定义 status
#define PTHREAD_STA_RUN     0x01
#define PTHREAD_STA_BUSY    0x02



typedef struct  {
    /**
     * 1 队列任务,放入队列,排队等待执行
     * 2 独占任务,将会查找空闲线程,执行
     * 3 
     */
    LINK_HEAD task_queue;
    LINK_HEAD task_lists;
    //
    THREAD_ENTRY threads[MAX_THREAD_COUNT];
    unsigned int thread_count;
} TASK_ENTRY;

int start_task_core(TASK_ENTRY** entry, unsigned int threadn);
void stop_task_core(TASK_ENTRY* entry);
