
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "queue.h"

/**
 * @brief Create a new queue
 * @param size Fixed total amount of elements allowed
 * in the queue at one time
 * @return queue pointer or NULL
 */
queue *queueInit(size_t size) {
    queue *newQueue = calloc(1, sizeof(queue));
    if (newQueue == NULL)
        return NULL;

    newQueue->head = -1;
    newQueue->tail = -1;
    newQueue->size = size;
    newQueue->items = calloc(size, sizeof(bool));
    if (newQueue->items == NULL) {
        free(newQueue);
        return NULL;
    }

    return newQueue;
}

/**
 * @brief Add an element to the back of the queue
 * @param queue     queue to add to
 * @param newItem   item to add to queue
 * @return OPERATION_SUCCESS
 */
STATUS_CODE queueEnqueue(queue *queue, bool newItem) {
    QUEUE_NULL_CHECK(queue);

    if (queue->count == queue->size)
        return QUEUE_FULL;

    size_t newTail = (queue->tail + 1) % (queue->size);

    if (queue->head == -1)
        queue->head = 0;
    
    queue->tail = newTail;
    queue->items[queue->tail] = newItem;
    queue->count++;

    return OPERATION_SUCCESS;
}

/**
 * @brief Remove and return an element from the front of the queue
 * @param queue         queue to remove from
 * @param returnedItem  item being dequeued (returned)
 * @return 
 */
STATUS_CODE queueDequeue(queue *queue, bool *returnedItem) {
    QUEUE_NULL_CHECK(queue);
    
    if (queue->count == 0)
        return QUEUE_UNDERFLOW;

    size_t newHead = (queue->head + 1) % (queue->size);

    bool item = queue->items[queue->head];
    queue->items[queue->head] = false;
    queue->head = newHead;
    queue->count--;

    *returnedItem = item;

    return OPERATION_SUCCESS;
}

// Return current items in the queue
size_t queueGetCurrentLength(queue *queue) {
    return queue->count;
}

// Print every queue entry
void queuePrint(queue *queue) {
    #ifdef PRINT_ENABLE

    for (size_t i = 0; i < queue->size; i++) {
        PRINTF("%d ", queue->items[i]);
    }
    
    PRINTF("\n");

    #endif // PRINT_ENABLE
}

void freeQueue(queue *queue) {
    if (queue != NULL) {
        if (queue->items != NULL)
            free(queue->items);
        
        free(queue);
    }
}

/* int main() {
    queue *newQueue = queueInit(7);
    queuePrint(newQueue);

    for (int i = 0; i < 8; i++) {
        queueEnqueue(newQueue, true);
        queuePrint(newQueue);
    }

    bool dequeueVal;
    queueDequeue(newQueue, &dequeueVal);
    queuePrint(newQueue);
    PRINTF("Dequeue Val: %d\n", dequeueVal);
    
    for (int i = 0; i < 3; i++) {
        queueEnqueue(newQueue, true);
        queuePrint(newQueue);
    }

    for (int i = 0; i < 8; i++) {
        queueDequeue(newQueue, &dequeueVal);
        queuePrint(newQueue);
        PRINTF("Dequeue Val: %d\n", dequeueVal);
    }

    queueEnqueue(newQueue, true);
    queuePrint(newQueue);
    queueEnqueue(newQueue, true);
    queuePrint(newQueue);
} */
