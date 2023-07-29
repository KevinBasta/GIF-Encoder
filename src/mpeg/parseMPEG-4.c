#ifndef MPEG_HEAD
    #define MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    //#include <string.h>
    #include <math.h>
    #include <time.h>

    #include "types.h"
    #include "typeUtility.h"
    #include "printUtility.h"
    #include "endianUtility.h"
    #include "bitUtility.h"
    #include "linkedList.h"
    #include "AVCUtility.h"

    #include "parseMPEG-4.h"
    #include "decodeMPEG-4.h"
    #include "decodeAVC.h"
    #include "processMPEG-4.h"
#endif


MPEG_Data *getMpegData(char *fileName) { 
    float startTime = (float)clock()/CLOCKS_PER_SEC;
    
    linkedList *topBoxesLL = initLinkedList();
    MPEG_Data *videoData = (MPEG_Data*) malloc(sizeof(MPEG_Data));
    

    printf("----------TOP LEVEL----------\n");
    readMainBoxes(fileName, topBoxesLL); // fopen takes in a relative path from executable location
    printAllBoxesLinkedList(topBoxesLL);

    // MDAT
    videoData->mdatBox = getBoxFromLinkedList(topBoxesLL, "mdat");
    videoData->mdatDataOffsetInFile = getOffsetToBoxLinkedList(topBoxesLL, "mdat") + BOX_HEADER_SIZE;
    //videoData->topBoxesLL = topBoxesLL;


    printf("----------MOOV LEVEL----------\n");
    box *moovBox = getBoxFromLinkedList(topBoxesLL, "moov");
    
    linkedList *moovLL = initLinkedList();
    parseChildBoxes(moovBox, moovLL);
    printAllBoxesLinkedList(moovLL);


    printf(">>>----------MVHD LEVEL----------<<<\n");
    box *mvhdBox = getBoxFromLinkedList(moovLL, "mvhd");
    mvhdParseBox(mvhdBox, videoData);


    printf("----------TRAK LEVEL----------\n");
    box *trakBox = getVideTrak(moovLL);

    linkedList *trakLL = initLinkedList();
    parseChildBoxes(trakBox, trakLL);
    printAllBoxesLinkedList(trakLL);


    printf(">>>----------TKHD LEVEL----------<<<\n");
    box *tkhdBox = getBoxFromLinkedList(trakLL, "tkhd");
    tkhdParseBox(tkhdBox, videoData);


    printf(">>>----------EDTS LEVEL----------<<<\n");
    box *edtsBox = getBoxFromLinkedList(trakLL, "edts");
    edtsParseBox(edtsBox, videoData);


    printf("----------MDIA LEVEL----------\n");
    box *mdia = getBoxFromLinkedList(trakLL, "mdia");

    linkedList *mdiaLL = initLinkedList();
    parseChildBoxes(mdia, mdiaLL);
    printAllBoxesLinkedList(mdiaLL);


    printf(">>>----------MDHD LEVEL----------<<<\n");
    box *mdhdBox = getBoxFromLinkedList(mdiaLL, "mdhd");
    mdhdParseBox(mdhdBox, videoData);


    printf(">>>----------HDLR LEVEL----------<<<\n");
    box *hdlrBox = getBoxFromLinkedList(mdiaLL, "hdlr");
    hdlrParseBox(hdlrBox);


    printf("----------MINF LEVEL----------\n");
    box *minf = getBoxFromLinkedList(mdiaLL, "minf");

    linkedList *minfLL = initLinkedList();
    parseChildBoxes(minf, minfLL);
    printAllBoxesLinkedList(minfLL);


    printf(">>>----------DINF LEVEL----------<<<\n");
    box *dinfBox = getBoxFromLinkedList(minfLL, "dinf");
    dinfParseBox(dinfBox, videoData);


    printf("----------STBL LEVEL----------\n");
    box *stbl = getBoxFromLinkedList(minfLL, "stbl");

    linkedList *stblLL = initLinkedList();
    parseChildBoxes(stbl, stblLL);
    printAllBoxesLinkedList(stblLL);


    printf(">>>--------STBL CHILDREN LEVEL--------<<<\n");
    box *stco = getBoxFromLinkedList(stblLL, "stco");
    stcoParseBox(stco, videoData);
    box *stsz = getBoxFromLinkedList(stblLL, "stsz");
    stszParseBox(stsz, videoData);
    box *stsc = getBoxFromLinkedList(stblLL, "stsc");
    stscParseBox(stsc, videoData);
    box *stss = getBoxFromLinkedList(stblLL, "stss");
    stssParseBox(stss, videoData);
    box *ctts = getBoxFromLinkedList(stblLL, "ctts");
    cttsParseBox(ctts, videoData);
    box *stts = getBoxFromLinkedList(stblLL, "stts");
    sttsParseBox(stts, videoData);
    printf("=====================================\n");
    box *stsd = getBoxFromLinkedList(stblLL, "stsd");
    stsdParseBox(stsd, videoData);
    printf("=====================================\n");
    
    
    float endTime = (float)clock()/CLOCKS_PER_SEC;
    float timeElapsed = endTime - startTime;
    printf("DATA PARSING OPERATION elapsed: %f\n", timeElapsed);

    freeLinkedList(moovLL, "box");
    freeLinkedList(trakLL, "box");
    freeLinkedList(mdiaLL, "box");
    freeLinkedList(minfLL, "box");
    freeLinkedList(stblLL, "box");
    videoData->topBoxesLL = topBoxesLL; // for later freeing

    return videoData;
}


/**
 * @brief get the video trak
 * paths: moov->trak->mdia->hdlr
 * @param moovLL linked list to search
 * @return trak box with type vide
 */
box *getVideTrak(linkedList *moovLL) { 
    box *trakBox;

    while ((trakBox = getBoxFromLinkedList(moovLL, "trak")) != NULL) { 
        linkedList *trakLL = initLinkedList();
        parseChildBoxes(trakBox, trakLL);

        box *mdiaBox = getBoxFromLinkedList(trakLL, "mdia");
        linkedList *mdiaLL = initLinkedList();
        parseChildBoxes(mdiaBox, mdiaLL);

        box *hdlrBox = getBoxFromLinkedList(mdiaLL, "hdlr");
        if (compareNBytes(hdlrParseBox(hdlrBox), "vide", 4) == TRUE) { 
            // gather other info into MPEG_Data then
            freeLinkedList(trakLL, "box");
            freeLinkedList(mdiaLL, "box");
            return trakBox;
        }
    }

    return NULL;
}


box *parseSingleNestedChildBox(u8 *boxData, u32 *bytesRead) {
    // parsing size and type from header
    u8 *childBoxHeaderSize = copyNBytes(BOX_SIZE_SIZE, boxData, bytesRead);
    u8 *childBoxHeaderType = copyNBytes(BOX_TYPE_SIZE, boxData, bytesRead);
    
    // converting size to int and freeing char array
    u32 childFullBoxSize = bigEndianU8ArrToLittleEndianU32(childBoxHeaderSize);
    free(childBoxHeaderSize);
    
    // DEBUG printf("%u\t", *childFullBoxSize);
    // DEBUG printNBytes(childBoxHeaderType, 4,"", "\n");

    // reading body of childBox
    i32 childBoxDataSize = childFullBoxSize - 8;
    u8 *childBoxData = copyNBytes(childBoxDataSize, boxData, bytesRead);
    
    // Creating a box struct to store current box
    box *currentChildBoxRead = (box*) malloc(sizeof(box));
    currentChildBoxRead->boxSize = childFullBoxSize;
    currentChildBoxRead->boxType = childBoxHeaderType;
    currentChildBoxRead->boxData = childBoxData;

    return currentChildBoxRead;
}


void parseNestedChildBoxes(u8 *boxData, u32 *bytesRead, u32 endIndex, linkedList *list) {
    while (*bytesRead < endIndex) { 
        // Creating a box struct to store current box
        box *currentChildBoxRead = parseSingleNestedChildBox(boxData, bytesRead);

        // Storing the current box in the current Node and allocating memory for the next box
        appendNodeLinkedList(list, currentChildBoxRead);
        if (*bytesRead >= endIndex) {
            nullifyLastNodeLinkedList(list);
        }
    }
}


/**
 * @brief given a container box, reads the child boxes and
 * stores them in the given linkedList
 * @param *parentBox    -   the container box contianing child boxes
 * @param *list         -   the linkedList to store the boxes in
 *
 * @note NOT REFERENCING PARENT BOX FIELDS. currenlty copying the 
 * information from the parentBox (creating new allocations for each child field).
 */
void parseChildBoxes(box *parentBox, linkedList *list) {
    u32 boxDataSize = parentBox->boxSize - BOX_HEADER_SIZE;
    u8 *boxData = parentBox->boxData;

    u32 bytesRead = 0;
    parseNestedChildBoxes(boxData, &bytesRead, boxDataSize, list);
}


/**
 *  reads the top level boxes in an MPEG-4 binary file format
 *  @param fileName:    a character array (includes '\0'), a valid 
 *                      path to an mp4 file that exists
 */
void readMainBoxes(u8 fileName[], linkedList *list) { 
    FILE *video = fopen(fileName, "rb");
    size_t chunksread;
    while (!feof(video)) { 
        // reading and storing the size of a box
        u8 *headerSize = (u8*) malloc(BOX_SIZE_SIZE);
        chunksread = fread(&headerSize[0], BOX_SIZE_SIZE, 1, video);
        // DEBUG printCharArrayBits(headerSize);

        /*
            Checking if at the end of file after reading the size part of a box header
            this ensures that only 4 extra bytes are read if already at end of file
        */
        if (feof(video)) { // can also use if chunksread == 0
            free(headerSize);
            nullifyLastNodeLinkedList(list);
            //DEBUG printf("end of file reached\n");
            break;
        }

        // reading and storing the type of a box
        u8 *headerType = (u8*) malloc(BOX_TYPE_SIZE);
        chunksread = fread(&headerType[0], BOX_TYPE_SIZE, 1, video);
        
        // translating the headerSize binary into an integer
        u32 fullBoxSize = bigEndianCharToLittleEndianUnsignedInt(headerSize);
        free(headerSize); // freeing the char malloc for header size
        
        
        // DEBUG printBits(sizeof(int), fullBoxSize);
        // DEBUG printf("%u\n", *fullBoxSize);
        // DEBUG printf("%s\n", headerType);
        /* 
            FOR ADDING SUPPORT FOR BIGGER FILE SIZES WHEN THE 
            fullBoxSize is equal to 1, there is a field after the 
            type in the header that gives 8 bytes to store size
            u8 *extendedHeaderSize = (u8*) malloc(8); 
        */

       
        /*
            fullBoxSize includes the size of the header 
            (box header: 8 bytes {[size -> 4 bytes] [type -> 4 bytes]})
            needs to be subtracted from fullBoxSize before the body of the box is read
        */
        i32 boxDataSize = fullBoxSize - 8;
        u8 *boxData = (u8*) malloc(boxDataSize);
        chunksread = fread(boxData, boxDataSize, 1, video);
        

        // Creating a box struct to store current box
        box *currentBoxRead = (box*) malloc(sizeof(box));
        currentBoxRead->boxSize = fullBoxSize;
        currentBoxRead->boxType = headerType;
        currentBoxRead->boxData = boxData;

        /*
            Storing the current box in the current Node and allocating memory for the next box
            setting the last node to the current node 
            (this is for setting it's nextBoxNode to null incase there is no next node)
        */
        appendNodeLinkedList(list, currentBoxRead);
    }
}
