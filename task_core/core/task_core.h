#pragma onece
#ifdef __cplusplus
extern "C" {
#endif
#include "linkedlist/linkedlist.h"
#include <pthread.h>
#include <semaphore.h>

// 队列任务的回调函数，回调函数参数需要自己回收
typedef void (*COMMIT_TASK_CBK)(void* arg);
// 任务定义
typedef enum {
    TASK_TYPE_QUEUE = 0,    // 排队任务
    TASK_TYPE_ONECE,        // 单次执行任务
    TASK_TYPE_WHILE,        // 循环轮询任务
    TASK_TYPE_WAIT,         // 阻塞等待任务
} TASK_TYPE;
// 任务节点
typedef struct {
    LINK_NODE node;
    COMMIT_TASK_CBK cbk;
    void* arg;
} TASK_BODY;

// 线程定义
typedef void(*DO_TASK_FUNC)(void* arg);
// 任务线程实例结构体
typedef struct {
    // 
    pthread_t pid;
    unsigned int status;
    // 任务线程序号
    unsigned int NO;
    //
    DO_TASK_FUNC dofunc;
    void *task_entry;
    // 线程绑定的任务,如果是队列任务,不需要
    TASK_BODY *body;
    // 线程等待变量
    sem_t wait_task;
} THREAD_ENTRY;
// 定义最大的线程限制数量,可以修改成合理值
#define MAX_THREAD_COUNT    30
// 线程状态的定义 status
#define PTHREAD_STA_RUN     0x01    // 线程状态,运行中,线程启动后标记
#define PTHREAD_STA_BUSY    0x02    // 线程状态,执行任务中,等待前后切换
#define PTHREAD_STA_PICK    0x04    // 当前线程体被选中,将会被操作,遍历不允许选中

// 任务模块实例结构体
typedef struct  {
    /**
     * 1 队列任务,放入队列,排队等待执行
     * 2 独占任务,将会查找空闲线程,执行
     * 3 
     */
    LINK_HEAD *task_queue;
    LINK_HEAD *task_lists;
    // 开启线程实例
    THREAD_ENTRY threads[MAX_THREAD_COUNT];
    pthread_mutex_t lock_threads;
    // 实际开启线程数量
    unsigned int thread_count;
} TASK_ENTRY;

// 启动任务模块
// threadn 启动线程数量
int start_task_core(TASK_ENTRY** entry, unsigned int threadn);
// 停止任务模块
void stop_task_core(TASK_ENTRY* entry);
// 提交(异步)
int commit_task(TASK_ENTRY* entry, COMMIT_TASK_CBK task, void *arg, TASK_TYPE type);
// 提交(同步)
int commit_task_sync();
#ifdef __cplusplus
}
#endif
