
#ifndef GIF_ENCODER_QUEUE_H
#define GIF_ENCODER_QUEUE_H

#include <stdbool.h>

#include <stdint.h>
#include "main.h"

typedef struct gif_queue {
    size_t size;
    size_t count;
    long head;
    long tail;
    bool *items;
} gif_queue;

gif_queue *gif_queueInit(size_t size);
STATUS_CODE gif_queueEnqueue(gif_queue *queue, bool newItem);
STATUS_CODE gif_queueDequeue(gif_queue *queue, bool *returnedItem);
size_t gif_queueGetCurrentLength(gif_queue *queue);
void gif_queuePrint(gif_queue *queue);
void gif_freeQueue(gif_queue *queue);

#endif // GIF_ENCODER_QUEUE_H