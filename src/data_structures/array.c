
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "printUtility.h"

/**
 * @brief Decide a new size for an array in the context of realloc 
 * @param arr Array to return new size for
 * @return New size (number of entries) for the array
 */
static size_t arrayNewSize(array *arr) {
    return arr->size + (arr->size / 2);
}

/**
 * @brief Expand an array by using the realloc stdlib function
 * @param arr               Array to expand
 * @param newTotalEntries   The new size (number of entries) to expand to
 * @return OPERATION_SUCCESS or error code
 */
static STATUS_CODE arrayRealloc(array *arr, size_t newTotalEntries) {
    u32 *status = realloc(arr->items, newTotalEntries * sizeof(u32));
    
    if (status == NULL) {
        freeArray(arr);
        return ARRAY_REALLOC_FAILED;
    }
    
    arr->items = status;
    arr->size  = newTotalEntries;

    PRINTF("ARRAY REALLOC!\n");

    return OPERATION_SUCCESS;
}

/**
 * @brief Initialize a u32 array
 * @param size  Max number of elements to allow
 * @return Pointer to new array or NULL
 */
array *arrayInit(size_t size) {
    array *arr = calloc(1, sizeof(array));
    if (arr == NULL)
        return NULL;

    arr->items = calloc(size, sizeof(u32));
    if (arr->items == NULL) {
        free(arr);
        return NULL;
    }

    arr->size = size;
    arr->currentIndex = 0;
    
    return arr;
}

/**
 * @brief Append a u32 item to an array
 * @param arr   Array to append to
 * @param item  The item to append
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE arrayAppend(array *arr, u32 item) {     
    ARRAY_NULL_CHECK(arr);
    
    if (arr->currentIndex >= arr->size) {
        STATUS_CODE status = arrayRealloc(arr, arrayNewSize(arr));
        CHECKSTATUS(status);
    }

    arr->items[arr->currentIndex] = item;
    arr->currentIndex++;

    return OPERATION_SUCCESS;
}

/**
 * @brief Delete the last appended item from the array
 * @param arr Array to delete from
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE arrayPop(array *arr) {
    ARRAY_NULL_CHECK(arr);

    if (arr->currentIndex - 1 < 0)
        return ARRAY_UNDERFLOW;

    arr->items[arr->currentIndex] = 0;
    arr->currentIndex--;

    return OPERATION_SUCCESS;
}

// Get item at specified index
STATUS_CODE arrayGetItemAtIndex(array *arr, size_t index, u32 *itemReturn) {
    if (index > arr->currentIndex)
        return ARRAY_INDEX_UNDEFINED;

    *itemReturn = arr->items[index];

    return OPERATION_SUCCESS;
}

// Get current index item
u32 arrayGetItemAtCurrentIndex(array *arr) {
    return arr->items[arr->currentIndex];
}

// Get current index item and increment
u32 arrayGetIncrement(array *arr) {
    return arr->items[(arr->currentIndex)++];
}

/**
 * @brief Reset the populated part of the array to 0
 * @param arr Array to reset elements of
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE arrayReset(array *arr) {
    ARRAY_NULL_CHECK(arr);

    memset(arr->items, 0, arr->currentIndex);
    arr->currentIndex = 0;

    return OPERATION_SUCCESS;
}

/**
 * @brief Create a string out of the elements of the array separated the
 * 1 char long entrySeparator.
 * @param arr               Array to create string from
 * @param entrySeparator    Character to put between each array entry in the string
 * @return A '\0' terminated string (character array) or NULL
 */
char* arrayConcat(array *arr, char entrySeparator) {
    // Allocate space for 3 chars per int (because max is 255) and 1 comma per int
    char *concat = calloc((arr->currentIndex * 3) + arr->currentIndex, sizeof(char));
    if (concat == NULL)
        return NULL;

    size_t k = 0;
    for (size_t i = 0; i < arr->currentIndex; i += 1) {
        char *itemStr = intToString(arr->items[i], 6);
        if (itemStr == NULL)
            return NULL;
        
        for (size_t j = 0; itemStr[j] != '\0'; j++) {
            concat[k] = itemStr[j];
            k++;
        }
        
        concat[k] = entrySeparator;
        k++;

        free(itemStr);
    }
    concat[k - 1] = '\0';

    return concat;
}

/**
 * @brief Initialize an array struct from a normal stack array
 * @param stackArr  A pointer to a temporary stack array
 * @param size      Size of given stack array
 * @return Pointer to new array struct
 */
array *arrayInitFromStackArray(u8 stackArr[], size_t size) {
    STATUS_CODE status;

    array *heapArr = arrayInit(size);
    if (heapArr == NULL)
        return NULL;
    
    for (size_t i = 0; i < size; i++) {
        status = arrayAppend(heapArr, stackArr[i]);
        if (status != OPERATION_SUCCESS) {
            freeArray(heapArr);
            return NULL;
        }
    }

    heapArr->currentIndex = 0;

    return heapArr;
}

// Print an array's items up to the last populated entry
void arrayPrint(array *arr) {
    #ifdef PRINT_ENABLE

    size_t index = 0;

    for (size_t i = 0; i < arr->currentIndex; i++) {
        PRINTF("%d ", arr->items[i]);
        index++;
        if (index % 24 == 0) {
            PRINTF("\n");
        }
    }

    PRINTF("\n");
    
    #endif // PRINT_ENABLE
}

// Free all of an array's entries
void freeArray(array *arr) {
    if (arr != NULL) {
        if (arr->items != NULL)
            free(arr->items);

        free(arr);
    }
}
