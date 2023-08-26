
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
u32 arrayGetItem(array *arr, size_t index);
void arrayReset(array *arr);
char* arrayConcat(array *arr, char entrySeparator);

void arrayPrint(array *arr);
void freeArray (array *arr);

#endif // ARRAY_H