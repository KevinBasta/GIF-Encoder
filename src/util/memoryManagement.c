
#include <stdlib.h>

#include <stdint.h>
#include "main.h"

#include "typesGIF.h"
#include "GIFCodeTable.h"
#include "array.h"
#include "typesStorage.h"

u8 isNULL(void *data) {
    if (data == NULL) { 
        return TRUE;
    } else {
        return FALSE;
    }
}

void safeFree(void *data) { 
    if (data != NULL) {
        free(data);
    }
}

void freeGifData(GIF_Data *gifData) {

    if (gifData->globalColorTable != NULL)
        freeColorTable(gifData->globalColorTable);
    
    if (gifData->localColorTable != NULL)
        freeColorTable(gifData->localColorTable);

    freeArray(gifData->indexStream);

    safeFree(gifData);
}
