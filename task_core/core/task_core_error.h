#ifndef __TASK_CORE_ERROR_H__
#define __TASK_CORE_ERROR_H__

#define TASK_CORE_CHECK_PARAM           1
#define TASK_CORE_MALLOC                        2   // 分配内存失败
#define TASK_CORE_THREAD_CREATE          3  // 创建线程失败
#define TASK_CORE_INIT_LINKED                   4   // 初始化链表失败
#define TASK_CORE_THREAD_BUSY              5    // 没有空闲的线程
#define TASK_CORE_INIT_MUTEX_LOCK     6   // 初始化线程锁失败
#define TASK_CORE_MUTEX_LOCK     7   // 加锁失败

#endif
