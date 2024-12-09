#include "task_core.h"
#include "task_core_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// 处理其他提交任务
static void hand_other_task(void* arg) {
    THREAD_ENTRY *ppthread = (THREAD_ENTRY*) arg;
    //TASK_ENTRY *entry = (TASK_ENTRY *)(ppthread->task_entry);

    //printf("线程 %d 处理其他任务\n", ppthread->NO);
    //printf("ttttttt1  %d\n", ppthread->NO);
    ppthread->body->cbk(ppthread->body->arg);
    //printf("ttttttt2  %d\n", ppthread->NO);
    // 执行任务结束,清理任务
    free(ppthread->body);
    ppthread->body = NULL;
}
// 处理队列任务实现
static void hand_queue_task(void* arg) {
    THREAD_ENTRY *ppthread = (THREAD_ENTRY*) arg;
    TASK_ENTRY *entry = (TASK_ENTRY *)(ppthread->task_entry);
    LINK_NODE *node = NULL;
    int ret = 0;

    //printf("线程 %d 处理队列任务, 任务数量 %d\n", ppthread->NO, entry->task_queue->nodecount);

    // 加锁
    // remove_lknode
    // 解锁
    while (0==(ret=remove_lknode(entry->task_queue, 1, &node)))
    {
        // 获取任务实例,处理任务
        TASK_BODY* body = CONTAINER_OF(node, TASK_BODY, node);
        body->cbk(body->arg);
        if (NULL!=body->arg) {
            printf("free arg\n");
            free(body->arg);
            body->arg = NULL;
        }
        // 销毁
        free(body);
    }
}

static void* do_thread(void* arg) {
    // 获取线程信息
    THREAD_ENTRY *ppthread = (THREAD_ENTRY*) arg;
    TASK_ENTRY *entry = (TASK_ENTRY *)(ppthread->task_entry);
    printf("run thread %d\n", ppthread->NO);

    // 标记线程运行
    ppthread->status |= PTHREAD_STA_RUN;
    while (1)
    {
        ppthread->status &= (~PTHREAD_STA_BUSY);
        // 修改PTHREAD_STA_PICK比较关键,加锁保护
        if (0!=pthread_mutex_trylock(&entry->lock_threads)) {
            //printf("获取锁失败...56, 将会一直等待\n");
            usleep(10000);
            pthread_mutex_lock(&entry->lock_threads);
        }
        // 每次执行完提交的任务之后,自动重新标记为可以选中,等待新任务
        ppthread->status &= (~PTHREAD_STA_PICK);
        pthread_mutex_unlock(&entry->lock_threads);

        // 等待开放线程（线程空闲）
        sem_wait(&(ppthread->wait_task));
        ppthread->status |= PTHREAD_STA_BUSY;

        if (NULL != ppthread->dofunc)
            ppthread->dofunc(ppthread);
    }

    return NULL;
}
// 停止所有的线程,遍历数组
static void clear_pthreads(TASK_ENTRY* entry) {
    if (NULL==entry)
        return;
    THREAD_ENTRY *ppthread = entry->threads;
    // 对线程数组加锁
    for (unsigned int i = 0; i < entry->thread_count;i++) {
        //printf("xxxx %d  %x\n", i,  ppthread[i].status);
        if (!(ppthread[i].status & PTHREAD_STA_RUN))
            continue;
        //printf("停止线程\n");
        pthread_cancel(ppthread[i].pid);
        //printf("回收线程\n");
        pthread_join(ppthread[i].pid, NULL);
        printf("pthread_join ok %d\n", ppthread[i].NO);

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

    // 初始化 TASK_ENTRY
    // 初始化链表x2
    ret = init_linkedlist(&(pptask->task_queue));
    if (ret<0) {
        ret = -TASK_CORE_INIT_LINKED;
        goto err_task_queue;
    }
    ret = init_linkedlist(&(pptask->task_lists));
    if (ret<0) {
        ret = -TASK_CORE_INIT_LINKED;
        goto err_task_lists;
    }

    if (threadn>MAX_THREAD_COUNT)
        threadn = MAX_THREAD_COUNT;

    THREAD_ENTRY *ppthread = pptask->threads;
    // 创建任务线程
    unsigned int i = 0;
    for (; i < threadn; i++) {
        ppthread[i].NO = i;
        ppthread[i].task_entry = pptask;
        sem_init(&(ppthread[i].wait_task), 0, 0);
        // 启动线程
        ret = pthread_create(&(ppthread[i].pid), NULL, do_thread, ppthread+i);
        if (0!=ret) {
            ret = -TASK_CORE_THREAD_CREATE;
            goto err_pthread_c;
        }
        // 指定默认回调
        ppthread[i].dofunc = hand_other_task;
        // 等待启动完成
        while (!(ppthread[i].status&PTHREAD_STA_RUN)) {
            //printf("等待启动 %d\n", ppthread[i].NO);
            usleep(10000);
        }
    }
    pptask->thread_count = i ;
    printf("start thread num %d\n", pptask->thread_count);

    // 第一个线程用于检查队列任务,初始化,开放
    ppthread[0].dofunc = hand_queue_task;

    // 初始化线程实例的锁
    ret = pthread_mutex_init(&pptask->lock_threads, NULL);
    if (0!=ret) {
        ret = -TASK_CORE_INIT_MUTEX_LOCK;
        goto err_pthread_c;
    }

    // 返回模块实例
    *entry = pptask;
    return 0;

err_pthread_c:
    clear_pthreads(pptask); 
    free_linkedlist(pptask->task_lists);
err_task_lists:
    free_linkedlist(pptask->task_queue);
err_task_queue:
    free(pptask);
    return ret;
}

void stop_task_core(TASK_ENTRY* entry) {
    if (NULL==entry)
        return ;

    pthread_mutex_destroy(&entry->lock_threads);
    // 清理线程
    clear_pthreads(entry);
    // 清理链表
    CLEAR_LKLIST(entry->task_lists, TASK_BODY, node);
    free_linkedlist(entry->task_lists);
    CLEAR_LKLIST(entry->task_queue, TASK_BODY, node);
    free_linkedlist(entry->task_queue);
    // 
    free(entry);
}

static int find_thread_do(TASK_ENTRY* entry, TASK_BODY* body, TASK_TYPE type) {
    (void)type;
    // 获取空闲线程
    int ret = 0;
    unsigned int i = 1;
    THREAD_ENTRY *ppthread = NULL;
    if (NULL==entry || NULL==body) {
        ret = -TASK_CORE_CHECK_PARAM;
        goto err_exit;
    }

    // 加锁
    if (0!=pthread_mutex_trylock(&entry->lock_threads)) {
        printf("lock fail, wait...\n");
        usleep(10000);
        if (0 != pthread_mutex_trylock(&entry->lock_threads)) {
            ret = -TASK_CORE_MUTEX_LOCK;
            goto err_exit;
        }
    }

    for (;i<entry->thread_count;i++) {
        ppthread = entry->threads+i;
        if ((ppthread->status)&PTHREAD_STA_PICK)
            continue;
        if ((ppthread->status)&PTHREAD_STA_RUN && !((ppthread->status)&PTHREAD_STA_BUSY)) {
            break;
        }
    }

    if (i>=entry->thread_count) {
        //printf("没有空闲线程\n");
        ret = -TASK_CORE_THREAD_BUSY;
        goto unlock_exit;
    }

    ppthread->status |= PTHREAD_STA_PICK;
    //printf("获取执行任务线程 %d\n", ppthread->NO);
    // 指定任务
    ppthread->body = body;
    // 解锁
    pthread_mutex_unlock(&entry->lock_threads);
    // 唤醒
    sem_post(&(ppthread->wait_task));

    return 0;

unlock_exit:
    // 解锁
    pthread_mutex_unlock(&entry->lock_threads);
err_exit:
    free(body);
    return ret;
}

// 回调函数参数需要自己回收
int commit_task(TASK_ENTRY* entry, COMMIT_TASK_CBK task, void* arg, TASK_TYPE type) {
    if (NULL==entry || NULL==task)
        return -TASK_CORE_CHECK_PARAM;

    TASK_BODY *body = (TASK_BODY *)calloc(1, sizeof(TASK_BODY));
    if (NULL==body)
        return -TASK_CORE_MALLOC;

    body->cbk = task;
    body->arg = arg;

    switch (type) {
    case TASK_TYPE_ONECE:
    case TASK_TYPE_WHILE:
    case TASK_TYPE_WAIT:
        // 查找空闲线程，绑定任务
        find_thread_do(entry, body, type);
        break;
    case TASK_TYPE_QUEUE:
        // 默认为队列任务
    default:
        // 加入链表
        if (insert_tail(entry->task_queue, &(body->node))<0) {
            // 插入失败
            free(body);
            break;
        }
        //printf("队列任务数量 %d\n", entry->task_queue->nodecount);
        // 唤醒线程
        sem_post(&(entry->threads[0].wait_task));
        break;
    }

    return 0;
}
