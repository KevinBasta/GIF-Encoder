
#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "linkedlist.h"
#include "GIFInterface.h"
#include "searchUtility.h"

/**
 * @brief Create a linkedlist
 * @return linklist pointer
 */
gif_linkedlist *gif_linkedlistInit() {
    gif_linkedlist *list = calloc(1, sizeof(gif_linkedlist));
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
STATUS_CODE gif_linkedlistAppend(gif_linkedlist *list, void *item) {
    LINKED_LIST_NULL_CHECK(list);

    gif_node *newNode       = calloc(1, sizeof(gif_node));
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
 * @brief Append item to the tail of a linkedlist
 * @param list  list to append to
 * @param item  item to append
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE gif_linkedlistPrepend(gif_linkedlist *list, void *item) {
    LINKED_LIST_NULL_CHECK(list);

    gif_node *newNode       = calloc(1, sizeof(gif_node));
    if (newNode == NULL)
        return LINKED_LIST_NODE_NULL;
    
    newNode->item       = item;
    newNode->nextNode   = NULL;

    if (list->size == 0) {
        list->head        = newNode;
        list->tail        = list->head;

        list->currentIter = list->head;
    } else {
        newNode->nextNode = list->head;
        list->head = newNode;
        
        list->currentIter = list->head;
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
STATUS_CODE gif_linkedlistYield(gif_linkedlist *list, void **item) {
    LINKED_LIST_NULL_CHECK(list);

    if (list->currentIter == NULL) {
        *item = NULL;
    } else if (list->currentIter != NULL) { 
        *item = (void*) list->currentIter->item;
        list->currentIter = list->currentIter->nextNode;
    }

    PRINTF("item: %p\n", *item);

    return OPERATION_SUCCESS;
}

// Reset list's currentIter field
void gif_linkedlistResetIter(gif_linkedlist *list) {
    list->currentIter = list->head;
}

void gif_freeFrameLinkedList(gif_linkedlist *list) {
    GIFFrame **framesFreed = calloc(list->size, sizeof(GIFFrame*));
    size_t i = 0;

    // If a frame is in the list twice
    // it will make us find an early null
    // since subsequent uses would be null
    size_t entriesLeft = list->size;
    
    if (list != NULL) {
        gif_node *currentHead = list->head;

        while (entriesLeft > 0) {
            GIFFrame *item = currentHead->item;
            if (!gif_frameInArray(item, framesFreed, i)) {
                gif_freeFrame(item);
                framesFreed[i] = item;
                i++;
            }

            gif_node *nextHead = currentHead->nextNode;
            free(currentHead);
            
            currentHead = nextHead;
            entriesLeft--;
        }

        free(list);
    }

    free(framesFreed);
}

/* void main() {
    gif_linkedlist *test = gif_linkedlistInit();
    u32 *testVal = malloc(sizeof(u32));
    *testVal = 44;

    PRINTF("HI\n");
    gif_linkedlistAppend(test, testVal);

    u32 *yeildReturn = malloc(sizeof(u32));

    linkedlistYieldInt(test, &yeildReturn);
    PRINTF("hello %d\n", *yeildReturn);
} */