
#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFInterface.h"
#include "GIFColorTable.h"
#include "GIFCodeTable.h"

void freeGifData(GIFGlobalRecord *gifData) {
    if (gifData != NULL) {
        if (gifData->globalColorTable != NULL)
            freeColorTable(gifData->globalColorTable);
        
        if (gifData->localColorTable != NULL)
            freeColorTable(gifData->localColorTable);

        freeArray(gifData->indexStream);

        free(gifData);
    }
}
