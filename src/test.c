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
    assert(getNBits(1, 3, 0xFF) == 7);
    assert(getNBits(3, 5, 0x5C) == 3);
    assert(getNBits(9, 5, 0x5C) == 0);
    assert(getNBits(4, 1, 0x5C) == 0);
    assert(getNBits(1, 1, 0x8C) == 1);

    //u8 test = getNBits(1, 1, 0x8C);
    //printBits(&test, 1);

    printf("All Assertions Passed\n");
}

