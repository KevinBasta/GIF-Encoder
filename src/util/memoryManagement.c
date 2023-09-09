
#include <stdlib.h>

#include <stdbool.h>
#include "main.h"

bool isNULL(void *data) {
    if (data == NULL) { 
        return true;
    } else {
        return false;
    }
}

/**
 * @note Ensure proper usage, can lead
 * to memory leaks if misused (by calling
 * with a type that needs nested frees)
 */
void freeIfNotNULL(void *data) { 
    if (data != NULL) {
        free(data);
    }
}
