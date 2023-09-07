#ifndef COMMON_HEAD
    #define COMMON_HEAD
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <math.h>
    #include <time.h>
    #include "main.h"
#endif //COMMON_HEAD

#ifndef COMMON_TYPES
    #define COMMON_TYPES
    #include "typesGIF.h"
    #include "typesUtility.h"
#endif //COMMON_TYPES

#ifndef DATA_STRUCTURES
    #define DATA_STRUCTURES
    #include "hashmap.h"
    #include "array.h"
    #include "typesStorage.h"
#endif //DATA_STRUCTURES

#ifndef COMMON_UTIL
    #define COMMON_UTIL
    #include "bitUtility.h"
    #include "endianUtility.h"
    #include "printUtility.h"
    #include "memoryManagement.h"
#endif //COMMON_UTIL

#ifndef GIF_HEAD
    #define GIF_HEAD
    #include "GIFEncode.h"
    #include "GIFCodeTable.h"
    #include "GIFError.h"
#endif //GIF_HEAD

STATUS_CODE gifErrorOutput() {
    // creates a gif that displays
    // the word "ERROR!" in case the main
    // program goes wrong somewhere
    // also serves as a good test


    return OPERATION_SUCCESS;
}


int main(int argc, char **argv) { 
    // get path and video name from user

    // Use parseMPEG-4 interface to get video frame information
    // "pipe" that information into a the gif interface functional style

    createErrorGif();

    printf("end of script\n");
    return 0;
}