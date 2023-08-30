
#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdint.h>
#include "main.h"

typedef struct bitarray {
    size_t size;
    size_t currentIndex;
    u8 currentBit: 3; //0-7
    u8 *items;
} bitarray;




#endif // BIT_ARRAY_H