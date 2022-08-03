#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#ifdef __cplusplus
extern "C" {
#endif

#define LKLIST_ERR_CHECKPARAM   1   // ��������
#define LKLIST_ERR_CALLOCFAIL   2   // ִ��callocʧ��

typedef struct {
    LINK_NODE* pre;
    LINK_NODE* next;
} LINK_NODE;

typedef struct {
    LINK_NODE node;
    int nodecount;      // ����ڵ�����,������head
} LINK_HEAD;

// ��̬����һ������ͷ
#define INIT_LINKEDLIST(head) \
    LINK_HEAD head = { \
        {&head, &head}, \
        0, \
    } \

// ����һ������ͷ
int init_linkedlist(LINK_HEAD** head);
// �ͷ�����ͷ
int free_linkedlist(LINK_HEAD* head);
// ����һ��
// index��ʾ���ĸ��ط�����,
// index=0��ʾ��head�������,1��ʾ�ڵ�һ��node�����,-1��ʾ�ڵ�����һ��node����
int insert_lknode(LINK_HEAD* head, LINK_NODE* node, int index);
// ǰ��/���
int insert_head(LINK_HEAD* head, LINK_NODE* node);
int insert_tail(LINK_HEAD* head, LINK_NODE* node);
// ɾ��һ��
// 1��ʾ�ڵ�һ��node,-1��ʾ������һ��node
int remove_lknode(LINK_HEAD* head, int index, LINK_NODE** node);
// ����
typedef int (*hand_node)(LINK_NODE* node);
int foreach_lklist(LINK_HEAD* head, hand_node hand);
// head:����ͷָ��,node:�ڵ�ָ��
#define FOREACH_LKLIST(head, type, member) \
    LINK_NODE* _mnode = NULL; \
    for (_mnode=head->node.next;_mnode!=(&head->node);_mnode=node->next)
// �ṹ�����
// �������ڽṹ���еĵ�ַƫ��
#define OFFSETOF(type, member) ((size_t)&((type*)0)->member)
// ����node��ַ��ȡ�ṹ���ַ
// ptr:nodeָ��,type:ʵ��ṹ������,member:node�ڽṹ���������
#define CONTAINER_OF(ptr, type, member) \
({ \
    const typeof(((type*)0)->member) _tmpptr = (ptr); \
    (type*)((char*)ptr-OFFSETOF(type, member)); \
})
// �������
int clear_lklist(LINK_HEAD* head, hand_node hand);
#define CLEAR_LKLIST(head, type, member) ( \
    LINK_NODE* _mnode = NULL; \
    for (_mnode=head->node.next;_mnode!=(&head->node);_mnode=node->next) { \
        const (type*) _mdata = CONTAINER_OF(_mnode, type, member); \
        free(_mdata); \
    } \
    head->nodecount;) \


#ifdef __cplusplus
}
#endif
#endif

