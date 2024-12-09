#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

    if (0!=pthread_mutex_init(&phead->lock_linked, NULL)) {
        free(phead);
        return -LKLIST_ERR_INIT_MUTEXLOCK;
    }

    *head = phead;
    return 0;
}

int free_linkedlist(LINK_HEAD* head) {
    if (NULL==head)
        return -LKLIST_ERR_CHECKPARAM;

    pthread_mutex_destroy(&head->lock_linked);

    free(head);
    return 0;
}

int insert_lknode(LINK_HEAD* head, LINK_NODE* node, int index) {
    LINK_NODE* pnode = (LINK_NODE*)head;
    if (NULL==head || NULL==node)
        return -LKLIST_ERR_CHECKPARAM;

    if (index>(head->nodecount) || (-index)>(head->nodecount))
        return -LKLIST_ERR_CHECKPARAM;

    if (0!=pthread_mutex_trylock(&head->lock_linked)) {
        usleep(10000);
        if (0!=pthread_mutex_trylock(&head->lock_linked))
            return -LKLIST_ERR_LOCK;
    }
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

    head->nodecount++;
    pthread_mutex_unlock(&head->lock_linked);

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

    if (0!=pthread_mutex_trylock(&head->lock_linked)) {
        usleep(10000);
        if (0!=pthread_mutex_trylock(&head->lock_linked))
            return -LKLIST_ERR_LOCK;
    }

    LINK_NODE* pnode = head->node.pre;

    // pnode --- node 
    node->next = pnode->next;
    pnode->next->pre = node;
    pnode->next = node;
    node->pre = pnode;

    head->nodecount++;
    pthread_mutex_unlock(&head->lock_linked);
    return 0;
}

int remove_lknode(LINK_HEAD* head, int index, LINK_NODE** node) {
    LINK_NODE* pnode = (LINK_NODE*)head;
    if (NULL==head || 0==index || NULL==node)
        return -LKLIST_ERR_CHECKPARAM;
    
    if (0!=pthread_mutex_trylock(&head->lock_linked)) {
        usleep(10000);
        if (0!=pthread_mutex_trylock(&head->lock_linked))
            return -LKLIST_ERR_LOCK;
    }

    if (index>(head->nodecount) || (-index)>(head->nodecount)) {
        pthread_mutex_unlock(&head->lock_linked);
        return -LKLIST_ERR_CHECKPARAM;
    }

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

    head->nodecount--;
    pthread_mutex_unlock(&head->lock_linked);
    return 0;
}

int foreach_lklist(LINK_HEAD* head, hand_node hand) {
    int ret = 0;
    if (NULL==head || NULL==hand)
        return -LKLIST_ERR_CHECKPARAM;

    if (0!=pthread_mutex_trylock(&head->lock_linked)) {
        usleep(10000);
        if (0!=pthread_mutex_trylock(&head->lock_linked))
            return -LKLIST_ERR_LOCK;
    }

    LINK_NODE* node = head->node.next;
    for (int i=0;i<head->nodecount;i++) {
        if ((ret=hand(node))<0)
            break;
        node = node->next;
    }

    pthread_mutex_unlock(&head->lock_linked);
    return ret;
}


