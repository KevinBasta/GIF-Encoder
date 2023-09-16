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

int main(int argc, char **argv) { 

    TIMER_INIT;
    TIMER_START();
    
    createErrorGif(CANVAS_NULL);
    // createTypingGIF("Hello! This GIF was generated by my GIF encoder!");
    // createTypingGIF("hello (not really hello, I can't see you). anyways!", false);
    // createTypingGIF("Aa Bb Cc Dd Ee Ff Gg Hh Ii Jj Kk Ll Mm Nn Oo Pp Qq Rr Ss Tt Uu Vv Ww Xx Yy Zz 0 1 2 3 4 5 6 7 8 9 ! ? / \\ \" ' [ ], ( ).", false);

    TIMER_END();
    
    printf("end of script\n");
    exit(0);
}