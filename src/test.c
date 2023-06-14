#include <assert.h>   
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "headers/types.h"
#include "headers/printUtility.h"
#include "headers/bitUtility.h"
#include "headers/linkedList.h"


int main() { 
    //assert(getNBits(1, 3, 0xFF) == 7);
    u8 res = getNBits(3, 5, 0x5C);
    printf("result: \n");
    printBits(&res, 1);
    //assert(getNBits(3, 5, 0x5C) == 3);
    //0101 1100
}

