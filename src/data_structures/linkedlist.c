
#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "linkedlist.h"
#include "GIFInterface.h"

/**
 * @brief Create a linkedlist
 * @return linklist pointer
 */
linkedlist *linkedlistInit() {
    linkedlist *list = calloc(1, sizeof(linkedlist));
    if (list == NULL)
        return NULL;

    list->size         = 0;
    list->head         = NULL;
    list->tail         = NULL;
    list->currentIter  = NULL;

    return list;
}

/**
 * @brief Append item to the tail of a linkedlist
 * @param list  list to append to
 * @param item  item to append
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE linkedlistAppend(linkedlist *list, void *item) {
    LINKED_LIST_NULL_CHECK(list);

    node *newNode       = calloc(1, sizeof(node));
    if (newNode == NULL)
        return LINKED_LIST_NODE_NULL;
    
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

/**
 * @brief Yield the next item currentIter points to
 * @param list  list to yield from
 * @param item  item returned (being yielded)
 * @return OPERATION_SUCCESS or error code
 */
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

// Reset list's currentIter field
void linkedlistResetIter(linkedlist *list) {
    list->currentIter = list->head;
}

void freeFrameLinkedList(linkedlist *list) {
    if (list != NULL) {
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