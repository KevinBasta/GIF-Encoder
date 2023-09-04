
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "printUtility.h"

// Private array helpers

static size_t arrayNewSize(array *arr) {
    return arr->size + (arr->size / 2);
}

static STATUS_CODE arrayRealloc(array *arr, size_t newSize) {
    u32 *status = realloc(arr->items, newSize * sizeof(u32));
    
    if (status == NULL) {
        freeArray(arr);
        return OPERATION_FAILED;
    }
    
    arr->items = status;
    arr->size = newSize;

    return OPERATION_SUCCESS;
}



// Array usage interface

array *arrayInit(size_t size) {
    array *arr = calloc(1, sizeof(array));

    arr->items = calloc(size, sizeof(u32));
    arr->size = size;
    arr->currentIndex = 0;
    
    return arr;
}

/**
 * @brief 
 * @param arr 
 * @param item 
 * @return OPERATION_SUCCESS or OPERATION_FAILED
 */
STATUS_CODE arrayAppend(array *arr, u32 item) {     
    if (arr == NULL) 
        return OPERATION_FAILED;
    
    if (arr->currentIndex >= arr->size) {
        STATUS_CODE status = arrayRealloc(arr, arrayNewSize(arr));
        CHECKSTATUS(status);
    }

    arr->items[arr->currentIndex] = item;
    arr->currentIndex++;

    return OPERATION_SUCCESS;
}

STATUS_CODE arrayPop(array *arr) {
    if (arr == NULL) 
        return OPERATION_FAILED;

    if (arr->currentIndex - 1 < 0)
        return OPERATION_FAILED;

    arr->items[arr->currentIndex] = 0;
    arr->currentIndex--;

    return OPERATION_SUCCESS;
}


u32 arrayGetItemAtIndex(array *arr, size_t index) {
    return arr->items[index];
}

u32 arrayGetItemAtCurrentIndex(array *arr) {
    return arr->items[arr->currentIndex];
}

u32 arrayGetIncrement(array *arr) {
    return arr->items[(arr->currentIndex)++];
}

void arrayReset(array *arr) {
    memset(arr->items, 0, arr->currentIndex);
    arr->currentIndex = 0;
}

char* arrayConcat(array *arr, char entrySeparator) {
    // printf("CONCAT: %d\n", arr->currentIndex *2 * sizeof(char));
    char *concat = calloc(arr->currentIndex * 2, sizeof(char));
    size_t k = 0;

    for (size_t i = 0; i < arr->currentIndex; i += 1) {
        char *itemStr = intToString(arr->items[i]);
        
        for (size_t j = 0; itemStr[j] != '\0'; j++) {
            concat[k] = itemStr[j];
            k++;
        }
        
        concat[k] = entrySeparator;
        k++;

        free(itemStr);
    }
    concat[(arr->currentIndex * 2) - 1] = '\0';
    //printf("TEST %s\n", concat);

    return concat;
}

// Array util interface

void arrayPrint(array *arr) {
    size_t index = 0;

    for (size_t i = 0; i < arr->currentIndex; i++) {
        printf("%d ", arr->items[i]);
        index++;
        if (index % 24 == 0) {
            printf("\n");
        }
    }

    printf("\n");
}

void freeArray(array *arr) {
    free(arr->items);
    free(arr);
}
