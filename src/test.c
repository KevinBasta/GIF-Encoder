
#include <assert.h>   

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "GIFInterface.h"
#include "GIFExtendedInterface.h"

#include "GIFError.h"
#include "gradientTest.h"
#include "localColorTableTest.h"
#include "maxColorIndexTest.h"
#include "smallTest.h"
#include "snakeTest.h"
#include "trafficLightTest.h"
#include "transparancyTest.h"
#include "typeTextTest.h"

#include "bitUtility.h"
#include "endianUtility.h"
#include "printUtility.h"

#define PRINT_STATUS_AND_EXIT(status) if (status != OPERATION_SUCCESS) { printf("Test Faild With Status %d\n", status); exit(1); }

int main() { 
    assert(gif_getNBits(0, 2, 0xFF)  == 7);
    assert(gif_getNBits(2, 4, 0x5C)  == 3);
    assert(gif_getNBits(0, 0, 0x8C)  == 1);
    assert(gif_getNBits(0, 7, 0x8C)  == 0x8C);
    assert(gif_getNBits(8, 4, 0x5C)  == 0);
    assert(gif_getNBits(-1, 4, 0x5C) == 0);
    assert(gif_getNBits(3, 0, 0x5C)  == 0);

    assert(gif_getNthBit(0, 0x8C)  == 1);
    assert(gif_getNthBit(2, 0xFF)  == 1);
    assert(gif_getNthBit(4, 0x5C)  == 1);
    assert(gif_getNthBit(6, 0x98)  == 0);
    assert(gif_getNthBit(7, 0x9E)  == 0);
    assert(gif_getNthBit(7, 0x9F)  == 1);
    assert(gif_getNthBit(8, 0x5C)  == 0);
    assert(gif_getNthBit(-1, 0x5C) == 0);

    assert(gif_getOccupiedBits(0x1)         == 1);
    assert(gif_getOccupiedBits(0x4)         == 3);
    assert(gif_getOccupiedBits(0x80)        == 8);
    assert(gif_getOccupiedBits(0x8000)      == 16);
    assert(gif_getOccupiedBits(0x80000000)  == 32);

    printf("All Assertions Passed\n");

    STATUS_CODE status;

    TIMER_INIT;
    TIMER_START();

    status = gif_createErrorGif(0);
    PRINT_STATUS_AND_EXIT(status);
    status = gif_createGradientTest();
    PRINT_STATUS_AND_EXIT(status);
    status = gif_createLocalColorTableTest();
    PRINT_STATUS_AND_EXIT(status);
    status = gif_createMaxColorIndexTest();
    PRINT_STATUS_AND_EXIT(status);
    status = gif_createSmallGif();
    PRINT_STATUS_AND_EXIT(status);
    status = gif_createSnakeTest();
    PRINT_STATUS_AND_EXIT(status);
    status = gif_createTrafficLightGif();
    PRINT_STATUS_AND_EXIT(status);
    status = gif_createTransparancyTest();
    PRINT_STATUS_AND_EXIT(status);
    status = gif_createTypingGIF("Aa Bb Cc Dd Ee Ff Gg Hh Ii Jj Kk Ll Mm Nn Oo Pp Qq Rr Ss Tt Uu Vv Ww Xx Yy Zz 0 1 2 3 4 5 6 7 8 9 ! ? / \\ \" ' [ ], ( ).", false);
    PRINT_STATUS_AND_EXIT(status);

    TIMER_END();

    printf("All Test GIFs Encoded Without Error\n");
    exit(0);
}

