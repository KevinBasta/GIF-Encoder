
#ifndef GIF_ENCODER_LINKED_LIST_H
#define GIF_ENCODER_LINKED_LIST_H

#include <stdint.h>
#include "main.h"

typedef struct gif_node { 
    void *item;
    void *nextNode;
} gif_node;

typedef struct gif_linkedlist {
    size_t size;
    gif_node *head;
    gif_node *tail;
    gif_node *currentIter;
} gif_linkedlist;

gif_linkedlist *gif_linkedlistInit();
STATUS_CODE gif_linkedlistAppend(gif_linkedlist *list, void *item);
STATUS_CODE gif_linkedlistPrepend(gif_linkedlist *list, void *item);
STATUS_CODE gif_linkedlistYield(gif_linkedlist *list, void **item);
void gif_linkedlistResetIter(gif_linkedlist *list);
void gif_freeFrameLinkedList(gif_linkedlist *list);


#endif // GIF_ENCODER_LINKED_LIST_H