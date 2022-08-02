#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#ifdef __cplusplus
extern "C" {
#endif

#define LKLIST_ERR_CHECKPARAM   1   // 参数错误
#define LKLIST_ERR_CALLOCFAIL   2   // 执行calloc失败

typedef struct {
    LINK_NODE* pre;
    LINK_NODE* next;
} LINK_NODE;

typedef struct {
    LINK_NODE node;
    int nodecount;      // 链表节点数量,不包括head
} LINK_HEAD;

// 静态申请一个链表头
#define INIT_LINKEDLIST(head) \
    struct LINK_HEAD head = { \
        {NULL, NULL,}, \
        0, \
    } \

// 创建一个链表头
int init_linkedlist(LINK_HEAD** head);
// 释放链表头
int free_linkedlist(LINK_HEAD* head);
// 插入一项
// index表示在哪个地方插入,
// index=0表示在head后面插入,1表示在第一个node后插入,-1表示在倒数第一个node插入
int insert_lknode(LINK_HEAD* head, LINK_NODE* node, int index);
// 前插/后插
int insert_head(LINK_HEAD* head, LINK_NODE* node);
int insert_tail(LINK_HEAD* head, LINK_NODE* node);
// 删除一项
// 1表示在第一个node,-1表示倒数第一个node
int remove_lknode(LINK_HEAD* head, int index, LINK_NODE** node);
// 遍历
typedef int (*hand_node)(LINK_NODE* node);
int foreach_lklist(LINK_HEAD* head, hand_node hand);

#ifdef __cplusplus
}
#endif
#endif

