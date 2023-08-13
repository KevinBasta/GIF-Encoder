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

#ifndef MPEG_HEAD
    #define MPEG_HEAD
    #include "parseMPEG-4.h"
    #include "decodeMPEG-4.h"
    #include "processMPEG-4.h"
    #include "decodeAVC.h"
    #include "AVCUtility.h"
#endif //MPEG_HEAD

#ifndef GIF_HEAD
    #define GIF_HEAD
    #include "encodeGIF.h"
#endif //GIF_HEAD

int main(int argc, char **argv) { 
    // get path and video name from user

    // Use parseMPEG-4 interface to get video frame information
    // "pipe" that information into a the gif interface functional style

    
    createGIF();
    MPEG_Data *videoData = getMpegData("local_files/op.mp4");
    createDisplayTimeToSampleTable(videoData);
    
    float startTime = (float)clock()/CLOCKS_PER_SEC;
    
    getVideoDataRangeByMediaTime(15, 20, videoData);
    
    float endTime = (float)clock()/CLOCKS_PER_SEC;
    float timeElapsed = endTime - startTime;
    printf("DATA PROCESSING OPERATION elapsed: %f\n", timeElapsed);
    
    
    //sampleSearchByRealTime(15, videoData);
    //keyFrameSearch(79, videoData);

    // free every linked list created
    //freeMpegData(videoData);
    printf("end of script\n");
    return 0;
}