
#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>
#include "main.h"

typedef struct array {
    size_t size;
    size_t currentIndex;
    u32 *items;
} array;

array *arrayInit(size_t size);
STATUS_CODE arrayAppend(array *arr, u32 item);
STATUS_CODE arrayPop(array *arr);
u32 arrayGetItemAtIndex(array *arr, size_t index);
u32 arrayGetItemAtCurrentIndex(array *arr);
u32 arrayGetIncrement(array *arr);
void arrayReset(array *arr);
char* arrayConcat(array *arr, char entrySeparator);
array *arrayInitFromStackArray(u8 *stackArr, size_t size);

void arrayPrint(array *arr);
void freeArray (array *arr);

#endif // ARRAY_H