
#ifndef MEMORY_MANAGE_H
#define MEMORY_MANAGE_H

#include <stdint.h>
#include "main.h"
#include "GIFInterface.h"

void freeGifData(GIFCanvas *gifData);
void freeIfNotNULL(void *data);
u8 isNULL(void *data);

#endif // MEMORY_MANAGE_H