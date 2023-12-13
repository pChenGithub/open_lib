#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

int init_linkedlist(LINK_HEAD** head) {
    LINK_HEAD *phead = NULL;
    if (NULL==head)
        return -LKLIST_ERR_CHECKPARAM;

    phead = calloc(1, sizeof(LINK_HEAD));
    if (NULL==phead)
        return -LKLIST_ERR_CALLOCFAIL;

    phead->node.next = & phead->node;
    phead->node.pre = & phead->node;
    phead->nodecount = 0;

    *head = phead;
    return 0;
}

int free_linkedlist(LINK_HEAD* head) {
    if (NULL==head)
        return -LKLIST_ERR_CHECKPARAM;

    free(head);
    return 0;
}

int insert_lknode(LINK_HEAD* head, LINK_NODE* node, int index) {
    LINK_NODE* pnode = (LINK_NODE*)head;
    if (NULL==head || NULL==node)
        return -LKLIST_ERR_CHECKPARAM;

    if (index>(head->nodecount) || (-index)>(head->nodecount))
        return -LKLIST_ERR_CHECKPARAM;

    if (index>0) {
        while (index--)
            pnode = pnode->next;
    } else if (index<0) {
        while (index++)
            pnode = pnode->pre;
    }
    
    // pnode --- node 
    node->next = pnode->next;
    pnode->next->pre = node;
    pnode->next = node;
    node->pre = pnode;

    return 0;
}

int insert_head(LINK_HEAD* head, LINK_NODE* node) {
    if (NULL==head || NULL==node)
        return -LKLIST_ERR_CHECKPARAM;
    return insert_lknode(head, node, 0);
}

int insert_tail(LINK_HEAD* head, LINK_NODE* node) {
    if (NULL==head || NULL==node)
        return -LKLIST_ERR_CHECKPARAM;
    LINK_NODE* pnode = head->node.pre;

    // pnode --- node 
    node->next = pnode->next;
    pnode->next->pre = node;
    pnode->next = node;
    node->pre = pnode;
    return 0;
}

int remove_lknode(LINK_HEAD* head, int index, LINK_NODE** node) {
    LINK_NODE* pnode = (LINK_NODE*)head;
    if (NULL==head || 0==index || NULL==node)
        return -LKLIST_ERR_CHECKPARAM;
    
    if (index>(head->nodecount) || (-index)>(head->nodecount))
        return -LKLIST_ERR_CHECKPARAM;

    if (index>0) {
        while (index--)
            pnode = pnode->next;
    } else if (index<0) {
        while (index++)
            pnode = pnode->pre;
    }

    pnode->pre->next = pnode->next;
    pnode->next->pre = pnode->pre;
    *node = pnode;
    return 0;
}

int foreach_lklist(LINK_HEAD* head, hand_node hand) {
    int ret = 0;
    if (NULL==head || NULL==hand)
        return -LKLIST_ERR_CHECKPARAM;

    int count = head->nodecount;
    LINK_NODE* node = head->node.next;
    for (int i=0;i<count;i++) {
        if ((ret=hand(node))<0)
            break;
        node = node->next;
    }

    return ret;
}


