
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "printUtility.h"

/**
 * @brief Only add one more entry to the array.
 * Can be optimized later by having a new field for
 * the actual size and the encoded size (where the
 * array would be expanded by much more).
 * @param arr Array to return new size for
 * @return New size (number of entries) for the array
 */
static size_t gif_arrayNewSize(gif_array *arr) {
    return arr->size + 1;
}

/**
 * @brief Expand an array by using the realloc stdlib function
 * @param arr               Array to expand
 * @param newTotalEntries   The new size (number of entries) to expand to
 * @return OPERATION_SUCCESS or error code
 */
static STATUS_CODE gif_arrayRealloc(gif_array *arr, size_t newTotalEntries) {
    u32 *status = realloc(arr->items, newTotalEntries * sizeof(u32));
    
    if (status == NULL) {
        gif_freeArray(arr);
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
gif_array *gif_arrayInit(size_t size) {
    gif_array *arr = calloc(1, sizeof(gif_array));
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
STATUS_CODE gif_arrayAppend(gif_array *arr, u32 item) {     
    ARRAY_NULL_CHECK(arr);
    
    if (arr->currentIndex >= arr->size) {
        STATUS_CODE status = gif_arrayRealloc(arr, gif_arrayNewSize(arr));
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
STATUS_CODE gif_arrayPop(gif_array *arr) {
    ARRAY_NULL_CHECK(arr);

    if (arr->currentIndex - 1 < 0)
        return ARRAY_UNDERFLOW;

    arr->items[arr->currentIndex] = 0;
    arr->currentIndex--;

    return OPERATION_SUCCESS;
}

// Get item at specified index
STATUS_CODE gif_arrayGetItemAtIndex(gif_array *arr, size_t index, u32 *itemReturn) {
    if (index > arr->currentIndex)
        return ARRAY_INDEX_UNDEFINED;

    *itemReturn = arr->items[index];

    return OPERATION_SUCCESS;
}

// Get current index item
u32 gif_arrayGetItemAtCurrentIndex(gif_array *arr) {
    return arr->items[arr->currentIndex];
}

// Get current index item and increment
u32 gif_arrayGetIncrement(gif_array *arr) {
    return arr->items[(arr->currentIndex)++];
}

/**
 * @brief Reset the populated part of the array to 0
 * @param arr Array to reset elements of
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE gif_arrayReset(gif_array *arr) {
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
gif_array* gif_arrayCopy(gif_array *arr) {
    gif_array *newArr = gif_arrayInit(arr->currentIndex);
    newArr->currentIndex = arr->currentIndex;
    memcpy(newArr->items, arr->items, (arr->currentIndex * sizeof(u32)));

    /* printf("=============\n");
    gif_arrayPrint(arr);
    gif_arrayPrint(newArr);
    printf("=============\n"); */

    return newArr;
}

/**
 * @brief Initialize an array struct from a normal stack array
 * @param stackArr  A pointer to a temporary stack array
 * @param size      Size of given stack array
 * @return Pointer to new array struct
 */
gif_array *gif_arrayInitFromStackArray(u8 stackArr[], size_t size) {
    STATUS_CODE status;

    gif_array *heapArr = gif_arrayInit(size);
    if (heapArr == NULL)
        return NULL;
    
    for (size_t i = 0; i < size; i++) {
        status = gif_arrayAppend(heapArr, stackArr[i]);
        if (status != OPERATION_SUCCESS) {
            gif_freeArray(heapArr);
            return NULL;
        }
    }

    heapArr->currentIndex = 0;

    return heapArr;
}

// Print an array's items up to the last populated entry
void gif_arrayPrint(gif_array *arr) {
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
void gif_freeArray(gif_array *arr) {
    if (arr != NULL) {
        if (arr->items != NULL)
            free(arr->items);

        free(arr);
    }
}
