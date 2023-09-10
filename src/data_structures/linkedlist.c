
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
        list->tail        = list->head;

        list->currentIter = list->head;
    } else {
        list->tail->nextNode = newNode;
        list->tail = list->tail->nextNode; 
    }

    list->size++;

    return OPERATION_SUCCESS;
}

STATUS_CODE linkedlistYield(linkedlist *list, void **item) {
    LINKED_LIST_NULL_CHECK(list);

    if (list->currentIter == NULL) {
        *item = NULL;
    } else if (list->currentIter != NULL) { 
        *item = (void*) list->currentIter->item;
        list->currentIter = list->currentIter->nextNode;
    }

    printf("item: %p\n", *item);

    return OPERATION_SUCCESS;
}

void linkedlistResetIter(linkedlist *list) {
    list->currentIter = list->head;
}

void freeFrameLinkedList(linkedlist *list) {
    linkedlistResetIter(list);

    node *currentHead = list->head;

    while (currentHead != NULL) {
        GIFFrame *item = currentHead->item;
        freeFrame(item);
    
        node *nextHead = currentHead->nextNode;
        free(currentHead);
        
        currentHead = nextHead;
    }

    free(list);
}

/* void main() {
    linkedlist *test = linkedlistInit();
    u32 *testVal = malloc(sizeof(u32));
    *testVal = 44;

    printf("HI\n");
    linkedlistAppend(test, testVal);

    u32 *yeildReturn = malloc(sizeof(u32));

    linkedlistYieldInt(test, &yeildReturn);
    printf("hello %d\n", *yeildReturn);


} */