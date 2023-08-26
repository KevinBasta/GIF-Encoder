
#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>
#include "main.h"

typedef struct array {
    size_t size;
    size_t currentIndex;
    u32 *items;
} array;

array *ArrayInit(size_t size);
STATUS_CODE ArrayAppend(array *arr, u32 item);
STATUS_CODE ArrayPop(array *arr);
u32 ArrayGetItem(array *arr, size_t index);
void ArrayReset(array *arr);
char* ArrayConcat(array *arr, char entrySeparator);

void ArrayPrint(array *arr);
void freeArray (array *arr);

#endif // ARRAY_H