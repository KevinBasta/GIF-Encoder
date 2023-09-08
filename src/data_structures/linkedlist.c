
#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "linkedlist.h"
#include "GIFInterface.h"

linkedlist *linkedlistInit() {
    linkedlist *list = calloc(1, sizeof(linkedlist));

    list->size         = 0;
    list->head         = NULL;
    list->tail         = NULL;
    list->currentIter  = NULL;

    return list;
}

STATUS_CODE linkedlistAppend(linkedlist *list, void *item) {
    node *newNode       = calloc(1, sizeof(node));
    newNode->item       = item;
    newNode->nextNode   = NULL;

    if (list->size == 0) {
        list->head        = newNode;
        list->tail        = newNode->nextNode;

        list->currentIter = list->head;
    } else {
        list->tail = newNode;
        list->tail = list->tail->nextNode; 
    }

    list->size++;

    return OPERATION_SUCCESS;
}

STATUS_CODE linkedlistYield(linkedlist *list, void *item) {
    item = list->currentIter->item;

    if (list->currentIter != NULL) {
        list->currentIter = list->currentIter->nextNode;
    }

    return OPERATION_SUCCESS;
}

STATUS_CODE linkedlistResetIter(linkedlist *list) {
    list->currentIter = list->head;
}

void freeFrameLinkedList(linkedlist *list) {
    linkedlistResetIter(list);

    GIFFrame *item;
    while (list->currentIter != NULL) {
        linkedlistYield(list, item);
        freeFrame(item);
    }

    free(list);
}