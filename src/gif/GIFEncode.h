
#ifndef GIF_ENCODE_H
#define GIF_ENCODE_H

#include <stdint.h>
#include "main.h"

#include "typesGIF.h"
#include "typesStorage.h"

STATUS_CODE createGIF(GIF_Data *gifData);
void addRGBArrayEntry(RGB *table, u32 index, u8 red, u8 green, u8 blue);

#endif // GIF_ENCODE_H