#ifndef MPEG_HEAD
    #define MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    //#include <string.h>
    #include <math.h>
    #include <time.h>

    #include "headers/types.h"
    #include "headers/typeUtility.h"
    #include "headers/printUtility.h"
    #include "headers/endianUtility.h"
    #include "headers/bitUtility.h"
    #include "headers/linkedList.h"
    #include "headers/AVCUtility.h"

    #include "headers/parseMPEG-4.h"
    #include "headers/decodeMPEG-4.h"
    #include "headers/decodeAVC.h"
    #include "headers/processMPEG-4.h"
#endif


int main(int argc, char **argv) { 
    // get path and video name from user

    // Use parseMPEG-4 interface to get video frame information
    // "pipe" that information into a the gif interface functional style

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
    freeMpegData(videoData);
    printf("end of script\n");
    return 0;
}