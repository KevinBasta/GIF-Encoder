
#ifndef MEMORY_MANAGE_H
#define MEMORY_MANAGE_H

#include <stdint.h>
#include "main.h"
#include "typesStorage.h"

void freeGifData(GIF_Data *gifData);
void safeFree(void *data);
u8 isNULL(void *data);

#endif // MEMORY_MANAGE_H