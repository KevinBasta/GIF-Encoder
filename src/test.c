#include <assert.h>   
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
//#include <CUnit/CUnit.h>

#include "headers/types.h"
#include "headers/printUtility.h"
#include "headers/bitUtility.h"
#include "headers/endianUtility.h"
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
    i32 bitOffset = 4;
    assert(countBitsToFirstNonZero(testArr, &bitOffset, &byteOffset, 9) == 11); byteOffset = 0; bitOffset = 7;
    assert(countBitsToFirstNonZero(testArr, &bitOffset, &byteOffset, 9) == 8); byteOffset = 0; bitOffset = 0;
    assert(countBitsToFirstNonZero(testArr, &bitOffset, &byteOffset, 9) == 15); byteOffset = 0; bitOffset = 2;
    assert(countBitsToFirstNonZero(testArr, &bitOffset, &byteOffset, 9) == 13); byteOffset = 0; bitOffset = 8;
    assert(countBitsToFirstNonZero(testArr, &bitOffset, &byteOffset, 9) == 15); byteOffset = 0; bitOffset = 0;

    testArr[0] = 0x08; testArr[1] = 0x08; // 0000 (1000 0000 1)000
    assert(bigEndianCharBitsToLittleEndianGeneralized(testArr, 4, 9) == 257);
    testArr[0] = 0x01; testArr[1] = 0x20; // 0000 000(1 001)0 0000
    assert(bigEndianCharBitsToLittleEndianGeneralized(testArr, 7, 4) == 9);
    testArr[0] = 0x01; testArr[1] = 0x20; // 0000 000(1 0010) 0000
    assert(bigEndianCharBitsToLittleEndianGeneralized(testArr, 7, 5) == 18);
    testArr[0] = 0x11; testArr[1] = 0x80; // 000(1 0001 1)000 0000
    assert(bigEndianCharBitsToLittleEndianGeneralized(testArr, 3, 6) == 35);
    testArr[0] = 0x6F; testArr[1] = 0x41; // (0110 1111 0100 0001)
    assert(bigEndianCharBitsToLittleEndianGeneralized(testArr, 0, 16) == 0x416F);
    testArr[0] = 0x6F; testArr[1] = 0x41; // (011 0111 1010 0000)1 
    assert(bigEndianCharBitsToLittleEndianGeneralized(testArr, 0, 15) == 0x37A0);
    testArr[0] = 0x6F; testArr[1] = 0x41; // 0110 1111 0100 0001 
    assert(bigEndianCharBitsToLittleEndianGeneralized(testArr, 0, 8) == 0x6F);
    

    
    //u8 test = getNBits(1, 1, 0x8C);
    //printBits(&test, 1);

    free(testArr);
    printf("All Assertions Passed\n");
}

