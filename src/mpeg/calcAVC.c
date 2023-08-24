#ifndef COMMON_HEAD
    #define COMMON_HEAD
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <math.h>
    #include <time.h>
#endif //COMMON_HEAD

#ifndef COMMON_TYPES
    #define COMMON_TYPES
    #include "main.h"
    #include "typesMPEG-4.h"
    #include "typesAVC.h"
    #include "linkedList.h"
    #include "typesStorage.h"
    #include "typesUtility.h"
#endif //COMMON_TYPES

#ifndef COMMON_UTIL
    #define COMMON_UTIL
    #include "bitUtility.h"
    #include "endianUtility.h"
    #include "printUtility.h"
    #include "memoryManagement.h"
#endif //COMMON_UTIL


/* i32 inverseRasterScan(i32 a, i32 b, i32 c, i32 d, i32 e) { 
    if (e == 0) { 
        return ((a % (d / b)) * b);
    } else if (e == 1) { 
        return ((a / (d / b)) * c);
    }

    return 0;
} */

