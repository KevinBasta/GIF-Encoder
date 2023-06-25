#ifndef MPEG_HEAD
    #define MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <math.h>
    #include <time.h>
    //#include <string.h>

    #include "../headers/types.h"
    #include "../headers/typeUtility.h"
    #include "../headers/printUtility.h"
    #include "../headers/endianUtility.h"
    #include "../headers/bitUtility.h"
    #include "../headers/linkedList.h"
    #include "../headers/parseMPEG-4.h"
    #include "../headers/decodeMPEG-4.h"
    #include "../headers/decodeAVC.h"
    #include "../headers/processMPEG-4.h"
#endif


i32 inverseRasterScan(i32 a, i32 b, i32 c, i32 d, i32 e) { 
    if (e == 0) { 
        return ((a % (d / b)) * b);
    } else if (e == 1) { 
        return ((a / (d / b)) * c);
    }

    return 0;
}

