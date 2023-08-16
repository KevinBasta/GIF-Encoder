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

array *initArray(size_t totalEntries) {
    array *arr = calloc(1, sizeof(array));

    arr->items = calloc(totalEntries, sizeof(u32));
    arr->totalEntries = totalEntries;
    arr->currentIndex = 0;
    
    return arr;
}

static size_t newTotalEntriesArray(array *arr) {
    return arr->totalEntries + (arr->totalEntries / 2);
}

static STATUS_CODE reallocArray(array *arr, size_t newTotalEntries) {
    printf("REACLLCLCOOCLC\n");
    printf("%p\n", arr->items);
    u32 *status = realloc(arr->items, newTotalEntries * sizeof(u32));
    
    if (status == NULL) {
        freeArray(arr);
        return OPERATION_FAILED;
    }
    
    arr->items = status;
    arr->totalEntries = newTotalEntries;

    printf("%p\n", arr->items);
    return OPERATION_SUCCESS;
}

STATUS_CODE appendItemArray(array *arr, u32 item) {     
    if (arr == NULL) 
        return OPERATION_FAILED;
    
    if (arr->currentIndex >= arr->totalEntries) {
        size_t status = reallocArray(arr, newTotalEntriesArray(arr));
        CHECK_NULL_RETURN(status);
    }

    arr->items[arr->currentIndex] = item;
    arr->currentIndex++;

    return OPERATION_SUCCESS;
}

void freeArray(array *arr) {
    free(arr->items);
    free(arr);
}
