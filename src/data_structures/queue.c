
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "queue.h"

queue *queueInit(size_t size) {
    queue *newQueue = calloc(1, sizeof(queue));
    newQueue->head = -1;
    newQueue->tail = -1;
    newQueue->size = size;
    newQueue->items = calloc(size, sizeof(bool));

    return newQueue;
}

STATUS_CODE queueEnqueue(queue *queue, bool newItem) {
    if (queue == NULL)
        return OPERATION_FAILED;

    if (queue->count == queue->size)
        return OPERATION_FAILED;

    size_t newTail = (queue->tail + 1) % (queue->size);

    if (queue->head == -1)
        queue->head = 0;
    
    queue->tail = newTail;
    queue->items[queue->tail] = newItem;
    queue->count++;

    return OPERATION_SUCCESS;
}

STATUS_CODE queueDequeue(queue *queue, bool *returnedItem) {
    if (queue == NULL)
        return OPERATION_FAILED;
    
    if (queue->count == 0)
        return OPERATION_FAILED;

    size_t newHead = (queue->head + 1) % (queue->size);

    bool item = queue->items[queue->head];
    queue->items[queue->head] = false;
    queue->head = newHead;
    queue->count--;

    *returnedItem = item;
}

size_t queueGetCurrentLength(queue *queue) {
    return queue->count;
}

void queuePrint(queue *queue) {
    for (size_t i = 0; i < queue->size; i++) {
        printf("%d ", queue->items[i]);
    }
    
    //printf("\t");
    //printf("count: %d head: %d tail: %d size: %d\n", queue->count, queue->head, queue->tail, queue->size);
    printf("\n");
}

void freeQueue(queue *queue) {
    free(queue->items);
    free(queue);
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
    printf("Dequeue Val: %d\n", dequeueVal);
    
    for (int i = 0; i < 3; i++) {
        queueEnqueue(newQueue, true);
        queuePrint(newQueue);
    }

    for (int i = 0; i < 8; i++) {
        queueDequeue(newQueue, &dequeueVal);
        queuePrint(newQueue);
        printf("Dequeue Val: %d\n", dequeueVal);
    }

    queueEnqueue(newQueue, true);
    queuePrint(newQueue);
    queueEnqueue(newQueue, true);
    queuePrint(newQueue);
} */
