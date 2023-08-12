#ifndef MPEG_HEAD
    #define MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <math.h>
    #include <time.h>
    //#include <string.h>

    #include "main.h"
    #include "typesStorage.h"
    #include "typesMPEG-4.h"
    #include "typesAVC.h"
    #include "linkedList.h"

    #include "typeUtility.h"
    #include "printUtility.h"
    #include "endianUtility.h"
    #include "bitUtility.h"
    #include "linkedList.h"
    #include "parseMPEG-4.h"
    #include "decodeMPEG-4.h"
    #include "decodeAVC.h"
    #include "processMPEG-4.h"
#endif


i32 inverseRasterScan(i32 a, i32 b, i32 c, i32 d, i32 e) { 
    if (e == 0) { 
        return ((a % (d / b)) * b);
    } else if (e == 1) { 
        return ((a / (d / b)) * c);
    }

    return 0;
}

