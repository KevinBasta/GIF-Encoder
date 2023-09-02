
#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#include <stdint.h>
#include "main.h"

typedef struct queue {
    size_t size;
    size_t count;
    long head;
    long tail;
    bool *items;
} queue;

queue *queueInit(size_t size);
STATUS_CODE queueEnqueue(queue *queue, bool newItem);
STATUS_CODE queueDequeue(queue *queue, bool *returnedItem);
size_t queueGetCurrentLength(queue *queue);
void queuePrint(queue *queue);

#endif // QUEUE_H