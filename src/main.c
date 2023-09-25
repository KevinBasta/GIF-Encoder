#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "main.h"

#include "searchUtility.h"

#include "hashmap.h"
#include "array.h"
#include "GIFInterface.h"

#include "bitUtility.h"
#include "endianUtility.h"
#include "printUtility.h"

#include "GIFEncode.h"
#include "GIFCodeTable.h"
#include "GIFError.h"
#include "typeTextTest.h"
#include "localColorTableTest.h"
#include "maxColorIndexTest.h"
#include "gradientTest.h"
#include "snakeTest.h"
#include "trafficLightTest.h"
#include "transparancyTest.h"
#include "smallTest.h"

int main(int argc, char **argv) { 
    STATUS_CODE status;

    TIMER_INIT;
    TIMER_START();
    
    status = gif_createErrorGif(CANVAS_NULL);
    // status = gif_createTypingGIF("This GIF was generated using my GIF encoder!", false);
    // status = gif_createTypingGIF("hello (not really hello, I can't see you). anyways!", false);
    // status = gif_createTypingGIF("public static void main string args! public. static. void. main. string[]. args!!", false);
    // status = gif_createTypingGIF("Aa Bb Cc Dd Ee Ff Gg Hh Ii Jj Kk Ll Mm Nn Oo Pp Qq Rr Ss Tt Uu Vv Ww Xx Yy Zz 0 1 2 3 4 5 6 7 8 9 ! ? / \\ \" ' [ ], ( ).", false);
    // status = gif_createLocalColorTableTest();
    // status = gif_createMaxColorIndexTest();
    // status = gif_createGradientTest();
    // status = gif_createSnakeTest();
    // status = createTrafficLightGif();
    // status = createTransparancyTest();
    // status = gif_createSmallGif();

    printf("Status code: %d\n", status);
    TIMER_END();
    
    printf("end of script\n");
    exit(0);
}