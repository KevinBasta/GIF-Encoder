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

extern int endiannessCheck;


STATUS_CODE encodeHeader(FILE *gif) {
    size_t status;

    const char header[3]    = "GIF";
    const char version[3]   = "89a";
    u32 nmemb = 1;

    status = fwrite(header, sizeof(header), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);
    status = fwrite(version, sizeof(header), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    return OPERATION_SUCCESS;
}

/**
 * @param canvasWidth   -   in pixels 
 * @param canvasHeight  -   in pixels
 */
STATUS_CODE encodeLogicalScreenDescriptor(FILE *gif, u16 canvasWidth, u16 canvasHeight) {
    size_t status;
    u32 nmemb = 1;

    canvasWidth = littleEndianU16(canvasWidth);
    fwrite(&canvasWidth, 2, nmemb, gif);

    return OPERATION_SUCCESS;
}



STATUS_CODE createGIF() {
    size_t status;
    
    FILE *gif = fopen("test.gif","wb");
    status = encodeHeader(gif);
    CHECKSTATUS(status);

    status = encodeLogicalScreenDescriptor(gif, 10, 10);

}

