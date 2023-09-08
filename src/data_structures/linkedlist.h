
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>
#include "main.h"

typedef struct node { 
    void *currentItem;
    void *nextNode;
} node;

typedef struct linkedlist {
    size_t size;
    node *head;
    node *tail;
    node *currentIter;
} linkedlist;



#endif // LINKED_LIST_H