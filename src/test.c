#include <assert.h>   
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
//#include <CUnit/CUnit.h>

#include "headers/types.h"
#include "headers/printUtility.h"
#include "headers/bitUtility.h"
#include "headers/linkedList.h"


int main() { 

    assert(getNBits(0, 2, 0xFF) == 7);
    assert(getNBits(2, 4, 0x5C) == 3);
    assert(getNBits(0, 0, 0x8C) == 1);
    assert(getNBits(0, 7, 0x8C) == 0x8C);
    assert(getNBits(8, 4, 0x5C) == 0);
    assert(getNBits(-1, 4, 0x5C) == 0);
    assert(getNBits(3, 0, 0x5C) == 0);

    assert(getNthBit(0, 0x8C) == 1);
    assert(getNthBit(2, 0xFF) == 1);
    assert(getNthBit(4, 0x5C) == 1);
    assert(getNthBit(6, 0x98) == 0);
    assert(getNthBit(7, 0x9E) == 0);
    assert(getNthBit(7, 0x9F) == 1);
    assert(getNthBit(8, 0x5C) == 0);
    assert(getNthBit(-1, 0x5C) == 0);

    u8 *testArr = (u8*) malloc(sizeof(u8) * 10);
    testArr[0] = 0x00; testArr[1] = 0x01; // 0000 0000 0000 0001
    u32 byteOffset = 0;
    assert(countBitsToFirstNonZero(testArr, 4, &byteOffset, 9) == 11); byteOffset = 0;
    assert(countBitsToFirstNonZero(testArr, 7, &byteOffset, 9) == 8); byteOffset = 0;
    assert(countBitsToFirstNonZero(testArr, 0, &byteOffset, 9) == 15); byteOffset = 0;
    assert(countBitsToFirstNonZero(testArr, 2, &byteOffset, 9) == 13); byteOffset = 0;
    assert(countBitsToFirstNonZero(testArr, 8, &byteOffset, 9) == 15); byteOffset = 0;
    assert(countBitsToFirstNonZero(testArr, -1, &byteOffset, 9) == 15); byteOffset = 0;



    //u8 test = getNBits(1, 1, 0x8C);
    //printBits(&test, 1);

    printf("All Assertions Passed\n");
}

