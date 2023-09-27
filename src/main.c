#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFExtendedInterface.h"
#include "GIFTransformations.h"

#include "GIFError.h"
#include "typeTextUtility.h"
#include "transparancyTest.h"

int main(int argc, char **argv) { 
    STATUS_CODE status;

    TIMER_INIT;
    TIMER_START();
    
     status = gif_createErrorGif(CANVAS_NULL);
    // status = gif_createTypingGIF("This GIF was generated using my GIF encoder!", false);
    // status = gif_createTypingGIF("hello (not really hello, I can't see you). anyways!", false);
    // status = gif_createTypingGIF("public static void main string args! public. static. void. main. string[]. args!!", false);
    // status = gif_createTypingGIF("Aa Bb Cc Dd Ee Ff Gg Hh Ii Jj Kk Ll Mm Nn Oo Pp Qq Rr Ss Tt Uu Vv Ww Xx Yy Zz 0 1 2 3 4 5 6 7 8 9 ! ? / \\ \" ' [ ], ( ).", false);

    printf("Status code: %d\n", status);
    TIMER_END();
    
    printf("end of script\n");
    exit(0);
}