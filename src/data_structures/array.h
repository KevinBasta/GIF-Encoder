
#ifndef GIF_ENCODER_ARRAY_H
#define GIF_ENCODER_ARRAY_H

#include <stdint.h>
#include "main.h"

typedef struct gif_array {
    size_t size;
    size_t currentIndex;
    u32 *items;
} gif_array;

gif_array *gif_arrayInit(size_t size);
STATUS_CODE gif_arrayAppend(gif_array *arr, u32 item);
STATUS_CODE gif_arrayPop(gif_array *arr);

STATUS_CODE gif_arrayGetItemAtIndex(gif_array *arr, size_t index, u32 *itemReturn);
u32         gif_arrayGetItemAtCurrentIndex(gif_array *arr);
u32         gif_arrayGetIncrement(gif_array *arr);

STATUS_CODE gif_arrayReset(gif_array *arr);
char        *gif_arrayConcat(gif_array *arr, char entrySeparator);
gif_array   *gif_arrayInitFromStackArray(u8 stackArr[], size_t size);

void gif_arrayPrint(gif_array *arr);
void gif_freeArray (gif_array *arr);

#endif // GIF_ENCODER_ARRAY_H