#ifndef COMMON_HEAD
    #define COMMON_HEAD
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <math.h>
    #include <time.h>
#endif //COMMON_HEAD

#ifndef COMMON_TYPES
    #define COMMON_TYPES
    #include "main.h"
    #include "array.h"
    #include "typesUtility.h"
#endif //COMMON_TYPES

#ifndef COMMON_UTIL
    #define COMMON_UTIL
    #include "printUtility.h"
#endif //COMMON_UTIL


// Private array helpers

static size_t arrayNewSize(array *arr) {
    return arr->size + (arr->size / 2);
}

static STATUS_CODE reallocArray(array *arr, size_t newSize) {
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

array *initArray(size_t size) {
    array *arr = calloc(1, sizeof(array));

    arr->items = calloc(size, sizeof(u32));
    arr->size = size;
    arr->currentIndex = 0;
    
    return arr;
}

STATUS_CODE appendArray(array *arr, u32 item) {     
    if (arr == NULL) 
        return OPERATION_FAILED;
    
    if (arr->currentIndex >= arr->size) {
        STATUS_CODE status = reallocArray(arr, arrayNewSize(arr));
        CHECKSTATUS(status);
    }

    arr->items[arr->currentIndex] = item;
    arr->currentIndex++;

    return OPERATION_SUCCESS;
}

STATUS_CODE popArray(array *arr) {
    if (arr == NULL) 
        return OPERATION_FAILED;

    if (arr->currentIndex - 1 < 0)
        return OPERATION_FAILED;

    arr->items[arr->currentIndex] = 0;
    arr->currentIndex--;

    return OPERATION_SUCCESS;
}


u32 getItemArray(array *arr, size_t index) {
    return arr->items[index];
}

void resetArray(array *arr) {
    memset(arr->items, 0, arr->size);
    arr->currentIndex = 0;
}

char* concatArray(array *arr, char entrySeparator) {
    char *concat = malloc(sizeof(char) * (arr->size * 2)); // needs to account for when entry separator isn't 1 char
    for (size_t i = 0; i < arr->size; i += 2) {
        char *itemStr = intToString(arr->items[i]);
        concat[i] = *(itemStr);
        free(itemStr);
        concat[i + 1] = entrySeparator;
    }

    concat[(arr->size * 2)] = '\0';
    printf("TEST %s\n", concat);

    return concat;
}

// Array util interface

void printArray(array *arr) {
    size_t index = 0;

    for (size_t i = 0; i < arr->currentIndex; i++) {
        printf("%d ", arr->items[i]);
        index++;
        if (index % 24 == 0) {
            printf("\n");
        }
    }
}

void freeArray(array *arr) {
    free(arr->items);
    free(arr);
}
