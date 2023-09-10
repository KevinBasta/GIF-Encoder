
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>
#include "main.h"

typedef struct node { 
    void *item;
    void *nextNode;
} node;

typedef struct linkedlist {
    size_t size;
    node *head;
    node *tail;
    node *currentIter;
} linkedlist;

linkedlist *linkedlistInit();
STATUS_CODE linkedlistAppend(linkedlist *list, void *item);
STATUS_CODE linkedlistYield(linkedlist *list, void **item);
void linkedlistResetIter(linkedlist *list);
void freeFrameLinkedList(linkedlist *list);


#endif // LINKED_LIST_H