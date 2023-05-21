#define MPEG_HEAD
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#include "headers/types.h"
#include "headers/printUtility.h"
#include "headers/bitUtility.h"
#include "headers/linkedList.h"

void readMainBoxes(char fileName[], linkedList *list);
void parseChildBoxes(box *parentBox, linkedList *list);
void parseNestedChildBoxes(char *boxData, unsigned int *bytesRead, unsigned int endIndex, linkedList *list);
box *parseSingleNestedChildBox(char *boxData, unsigned int *bytesRead);
void ftypParseBox(box *ftypBox);
void mvhdParseBox(box *mvhdBox, MPEG_Data *videoData);
void tkhdParseBox(box *trakBox, MPEG_Data *videoData);
void elstParseBox(box *elstBox, MPEG_Data *videoData);
void edtsParseBox(box *edtsBox, MPEG_Data *videoData);
char *hdlrParseBox(box *hdlrBox);
void vmhdParseBox(box *vmhdBox);
void dinfParseBox(box *dinfBox);
box *getVideTrak(linkedList *moovLL);

void stsdParseBox(box *stsdBox);
void stszParseBox(box *stszBox);
void stscParseBox(box *stscBox);
void stcoParseBox(box *stcoBox);
void sttsParseBox(box *sttsBox);
void stssParseBox(box *stssBox);
void cttsParseBox(box *cttsBox);
void videoMediaBox();

int main(int argc, char **argv) { 
    linkedList *topBoxesLL = initLinkedList();
    // fopen taken in a relative path from executable location
    readMainBoxes("local_files/op2.mp4", topBoxesLL);

    MPEG_Data *videoData = (MPEG_Data*) malloc(sizeof(MPEG_Data));


    printf("----------TOP LEVEL----------\n");
    printAllBoxesLinkedList(topBoxesLL);
    

    printf("----------MOOV LEVEL----------\n");
    box *moovBox = getBoxFromLinkedList(topBoxesLL, "moov");
    
    linkedList *moovLL = initLinkedList();
    parseChildBoxes(moovBox, moovLL);
    printAllBoxesLinkedList(moovLL);


    printf(">----------MVHD LEVEL----------\n");
    box *mvhdBox = getBoxFromLinkedList(moovLL, "mvhd");
    mvhdParseBox(mvhdBox, videoData);



    printf("----------TRAK LEVEL----------\n");
    box *trakBox = getVideTrak(moovLL);

    linkedList *trakLL = initLinkedList();
    parseChildBoxes(trakBox, trakLL);
    printAllBoxesLinkedList(trakLL);



    
    printf(">----------TKHD LEVEL----------\n");
    box *tkhdBox = getBoxFromLinkedList(trakLL, "tkhd");
    tkhdParseBox(tkhdBox, videoData);


    printf(">----------EDTS LEVEL----------\n");
    box *edtsBox = getBoxFromLinkedList(trakLL, "edts");
    edtsParseBox(edtsBox, videoData);




    printf("----------MDIA LEVEL----------\n");
    box *mdia = getBoxFromLinkedList(trakLL, "mdia");

    linkedList *mdiaLL = initLinkedList();
    parseChildBoxes(mdia, mdiaLL);
    printAllBoxesLinkedList(mdiaLL);


    printf("----------MINF LEVEL----------\n");
    box *minf = getBoxFromLinkedList(mdiaLL, "minf");

    linkedList *minfLL = initLinkedList();
    parseChildBoxes(minf, minfLL);
    printAllBoxesLinkedList(minfLL);


    printf("----------STBL LEVEL----------\n");
    box *stbl = getBoxFromLinkedList(minfLL, "stbl");

    linkedList *stblLL = initLinkedList();
    parseChildBoxes(stbl, stblLL);
    printAllBoxesLinkedList(stblLL);


    printf("--------STTS CHILD LEVEL--------\n");
    box *stco = getBoxFromLinkedList(stblLL, "stco");
    stcoParseBox(stco);
    box *stsz = getBoxFromLinkedList(stblLL, "stsz");
    stszParseBox(stsz);
    /* box *stsc = getBoxFromLinkedList(stblLL, "stsc");
    stscParseBox(stsc); */
    /* box *stss = getBoxFromLinkedList(stblLL, "stss");
    stssParseBox(stss); */
    /* box *ctts = getBoxFromLinkedList(stblLL, "ctts");
    cttsParseBox(ctts); */
    /* box *stts = getBoxFromLinkedList(stblLL, "stts");
    sttsParseBox(stts); */
    /* box *stsd = getBoxFromLinkedList(stblLL, "stsd");
    stsdParseBox(stsd); */

    



    /* printf("----------DINF LEVEL----------\n");
    box *dinf = getBoxFromLinkedList(minfLL, "dinf");
    dinfParseBox(dinf); */

    /* printf("\n----------HDLR LEVEL----------\n");
    box *hdlrBox = getBoxFromLinkedList(mdiaLL, "hdlr");
    hdlrParseBox(hdlrBox); // Establishes that it's a video track */
   

    /* printf("----------TRAK LEVEL----------\n");
    box *edts = getBoxFromLinkedList(trakHeadNode, "edts");
    Node *edtsHeadNode = parseChildBoxes(edts);
    printAllBoxesLinkedList(edtsHeadNode);
    printf("----------EDTS LEVEL----------\n");
    box *elst = getBoxFromLinkedList(edtsHeadNode, "elst");
    elstParseBox(elst); */
   


    /* printf("----------TOP LEVEL----------\n");
    box *mdat = getBoxFromLinkedList(headNode, "mdat");
    Node *mdatHeadNode = parseChildBoxes(mdat);
    printAllBoxesLinkedList(mdatHeadNode); */

    //tkhdParseBox(tkhd);

    //printf("%d\n", boxTypeEqual(headNode->currentBox->boxType, "ftyp"));
    //ftypParseBox(headNode->currentBox);
    

    // free every linked list created
    /* freeLinkedList(topBoxesLL, "box");
    freeLinkedList(moovLL, "box");
    freeLinkedList(trakLL, "box");
    freeLinkedList(mdiaLL, "box");
    freeLinkedList(minfLL, "box");
    freeLinkedList(stblLL, "box"); */
    printf("end of script\n");
    return 0;
}


/**
 * @brief 
 * 
 * paths - 
 * moov->trak->mdia->hdlr
 * 
 * @param moovLL 
 * @return 
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
            return trakBox;
        }
    }

    return NULL;
}


//https://developer.apple.com/library/archive/documentation/QuickTime/QTFF/QTFFChap2/qtff2.html#//apple_ref/doc/uid/TP40000939-CH204-25680
//the following child atoms are required: sample description, sample size, sample to chunk, and chunk offset
//If the sync sample atom is not present, all samples are implicitly sync samples.

/*
Child boxes of STBL (Sample Table Atom). These define Samples and Chunks in the file.

*/
void stsdParseBox(box *stsdBox) { //sample description required
    unsigned int boxSize = *(stsdBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = stsdBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);

    unsigned int *numberOfEntriesInt = charToUnsignedInt(numberOfEntries);
    // DEBUG printf("%d\n", *numberOfEntriesInt);
    
    linkedList sampleDescriptions;
    printf("entries: %d\n", *numberOfEntriesInt);
    for (int i = 0; i < *numberOfEntriesInt; i++) { 
        char *sampleDescriptionSizeChar = referenceNBytes(4, boxData, &bytesRead);
        unsigned int *sampleDescriptionSize = charToUnsignedInt(sampleDescriptionSizeChar);
        unsigned int absoluteEndOfSampleDescription = bytesRead + *sampleDescriptionSize - 4;
        printf("total %d\n", boxDataSize);

        char *dataFormat = referenceNBytes(4, boxData, &bytesRead);
        char *reserved = referenceNBytes(6, boxData, &bytesRead);
        char *dataReferenceIndex = referenceNBytes(2, boxData, &bytesRead); 
        int *dataReferenceIndexInt = bigEndianCharToLittleEndianBytedInt(dataReferenceIndex, 2); 

        printf("%d\n", *sampleDescriptionSize);
        printNBytes(dataFormat, 4, "", "\n");
        printf("%d\n", *dataReferenceIndexInt);

        // The following fields assume that this stsd box belongs to a video trak 
        char *version =         referenceNBytes(2, boxData, &bytesRead);
        char *revisionLevel =   referenceNBytes(2, boxData, &bytesRead);
        char *vendor =          referenceNBytes(4, boxData, &bytesRead);
        // the following return value passing is only permissible since the data is a 
        // reference in the bigger array and not separatly allocated memory that would need freeing
        int *temporalQuality =      bigEndianCharToLittleEndianBytedInt(referenceNBytes(4, boxData, &bytesRead), 4);
        int *spatialQuality =       bigEndianCharToLittleEndianBytedInt(referenceNBytes(4, boxData, &bytesRead), 4);
        int *width =                bigEndianCharToLittleEndianBytedInt(referenceNBytes(2, boxData, &bytesRead), 2);
        int *height =               bigEndianCharToLittleEndianBytedInt(referenceNBytes(2, boxData, &bytesRead), 2);

        char *horizontalResolutionChar =  referenceNBytes(4, boxData, &bytesRead);
        char *verticalResolutionChar =    referenceNBytes(4, boxData, &bytesRead);
        float *horizontalResolution =  bigEndianCharToLittleEndianFloat(horizontalResolutionChar);
        //float *verticalResolution =    bigEndianCharToLittleEndianFloat(verticalResolutionChar);
        
        int *dataSize =             bigEndianCharToLittleEndianBytedInt(referenceNBytes(4, boxData, &bytesRead), 4);
        int *frameCount =           bigEndianCharToLittleEndianBytedInt(referenceNBytes(2, boxData, &bytesRead), 2);
        
        char *compressorName =      referenceNBytes(4, boxData, &bytesRead);

        int *depth =              bigEndianCharToLittleEndianBytedInt(referenceNBytes(2, boxData, &bytesRead), 2);
        int *colorTableID =       bigEndianCharToLittleEndianBytedInt(referenceNBytes(2, boxData, &bytesRead), 2);


        printf("width height: %d %d\n", *width, *height);
        printf("%f\n", *horizontalResolution);
        //printf("horizontal vertical resolution: %f %f\n", *horizontalResolution, *verticalResolution);
        printBits(horizontalResolutionChar, 4);
        //printBits(verticalResolutionChar, 4);
        printBits(horizontalResolution, 4);
        //printBits(verticalResolution, 4);
        printf("frame count: %d\n", *frameCount);
        
        printf("read: %d, end: %d\n", bytesRead, absoluteEndOfSampleDescription);
        /*
            Unspecified 28 bytes of all 0 bits not mentioned in spesification
            the last 4 bytes contains some unknown non-zero bits
        */
        char *emptyFourBytes = referenceNBytes(4, boxData, &bytesRead);
        emptyFourBytes = referenceNBytes(4, boxData, &bytesRead);
        emptyFourBytes = referenceNBytes(4, boxData, &bytesRead);
        emptyFourBytes = referenceNBytes(4, boxData, &bytesRead);
        emptyFourBytes = referenceNBytes(4, boxData, &bytesRead);
        emptyFourBytes = referenceNBytes(4, boxData, &bytesRead);
        emptyFourBytes = referenceNBytes(4, boxData, &bytesRead);
        printBits(emptyFourBytes, 4);
        printNBytes(emptyFourBytes, 4, "", "\n");
        unsigned int *test = charToUnsignedInt(emptyFourBytes);
        printf("%d\n", *test);

        printf("read: %d, end: %d\n", bytesRead, absoluteEndOfSampleDescription);
        if (bytesRead != absoluteEndOfSampleDescription) {
            printf("not end\n"); 
            linkedList *stsdVideoExtention = initLinkedList();
            parseNestedChildBoxes(boxData, &bytesRead, absoluteEndOfSampleDescription, stsdVideoExtention);
            printAllBoxesLinkedList(stsdVideoExtention);

            // parse video sample description extensions
        }

    }
    

}

/* int isNullBytes(unsigned int numberOfBytes, char *data, int *index) { 
    for (int i = 0; i < numberOfBytes; i++) { 
        if (data[*index + i] != NULL) { 
            return FALSE;
        }
    }

    *index = *index + numberOfBytes;
    return TRUE;
} */


void stszParseBox(box *stszBox) { //sample size required
    unsigned int boxSize = *(stszBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = stszBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *sampleSize = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *sampleSizeInt = charToUnsignedInt(sampleSize);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *numberOfEntriesInt = charToUnsignedInt(numberOfEntries);
    printf("%d\n", *sampleSizeInt);
    printf("%d\n", *numberOfEntriesInt);
    printf("%u %u\n", boxDataSize, bytesRead);
    //for (int i = 1; i <= *numberOfEntriesInt; i++) { 
    for (int i = 1; i <= 10; i++) { 
        char *size = referenceNBytes(4, boxData, &bytesRead);
        unsigned int *sizeInt = charToUnsignedInt(size);

        printf("%d: %u\n", i, *sizeInt);
    
        // calloc array of number of entries * size of usngiend int
        // set each index 
    }
}

void stscParseBox(box *stscBox) { //sample to chunk required
    unsigned int boxSize = *(stscBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = stscBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *numberOfEntriesInt = charToUnsignedInt(numberOfEntries);
    printf("%d\n", *numberOfEntriesInt);
    printf("First chunk \t Samples per chunk \t Sample description ID\n");
    for (int i = 0; i < *numberOfEntriesInt; i++) { 
        char *firstChunk = referenceNBytes(4, boxData, &bytesRead); 
        char *samplesPerChunk = referenceNBytes(4, boxData, &bytesRead); 
        char *sampleDescriptionId = referenceNBytes(4, boxData, &bytesRead);

        unsigned int *firstChunkInt = charToUnsignedInt(firstChunk); 
        unsigned int *samplesPerChunkInt = charToUnsignedInt(samplesPerChunk); 
        unsigned int *sampleDescriptionIdInt = charToUnsignedInt(sampleDescriptionId); 
        printf("%d \t\t %d \t\t %d \n", *firstChunkInt, *samplesPerChunkInt, *sampleDescriptionIdInt);
    }

}

void stcoParseBox(box *stcoBox) { //chunk offset required
    unsigned int boxSize = *(stcoBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = stcoBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *numberOfEntriesInt = charToUnsignedInt(numberOfEntries);
    printf("%d\n", *numberOfEntriesInt);
    
    printf("Chunk: Offset\n");
    for (int i = 1; i <= 10; i++) {
        char *offset = referenceNBytes(4, boxData, &bytesRead);
        unsigned int *offsetInt = charToUnsignedInt(offset);

        printf("%d: %d\n", i, *offsetInt);
    }
}

void sttsParseBox(box *sttsBox) { //time to sample
    unsigned int boxSize = *(sttsBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = sttsBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *numberOfEntriesInt = charToUnsignedInt(numberOfEntries);
    printf("%d\n", *numberOfEntriesInt);
    printf("Sample Count \t\t Sample Duration\n");
    for (int i = 0; i < *numberOfEntriesInt; i++) { 
        char *sampleCount = referenceNBytes(4, boxData, &bytesRead);
        char *sampleDuration = referenceNBytes(4, boxData, &bytesRead);
        
        unsigned int *sampleCountInt = charToUnsignedInt(sampleCount);
        unsigned int *sampleDurationInt = charToUnsignedInt(sampleDuration);
        printf("%d \t\t %d\n", *sampleCountInt, *sampleDurationInt);
        free(sampleCountInt);
        free(sampleDurationInt);
    }

}

void stssParseBox(box *stssBox) { //sync sample
    unsigned int boxSize = *(stssBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = stssBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *numberOfEntriesInt = charToUnsignedInt(numberOfEntries);
    printf("%d\n", *numberOfEntriesInt);
    for (int i = 0; i < *numberOfEntriesInt; i++) { 
        char *entry = referenceNBytes(4, boxData, &bytesRead);
        unsigned int *entryInt = charToUnsignedInt(entry);
        printf("%d\n", *entryInt);
    }

}

void cttsParseBox(box *cttsBox) { //composition offset
    unsigned int boxSize = *(cttsBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = cttsBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *numberOfEntriesInt = charToUnsignedInt(numberOfEntries);
    printf("%d\n", *numberOfEntriesInt);
    printf("Sample Count \t\t Composition Offset\n");
    for (int i = 0; i < *numberOfEntriesInt; i++) { 
        char *sampleCount = referenceNBytes(4, boxData, &bytesRead);
        char *compositionOffset = referenceNBytes(4, boxData, &bytesRead);
        
        unsigned int *sampleCountInt = charToUnsignedInt(sampleCount);
        unsigned int *compositionOffsetInt = charToUnsignedInt(compositionOffset);
        printf("%d \t\t %d\n", *sampleCountInt, *compositionOffsetInt);
        free(sampleCountInt);
        free(compositionOffsetInt);
    }
}

void videoMediaBox() { 

}



/* void stsdParseBox(box *stsdBox) { 
    unsigned int boxSize = *(stsdBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = stsdBox->boxData;

} */






// NEEDS UPDATING 
void dinfParseBox(box *dinfBox) {
    unsigned int boxSize = *(dinfBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = dinfBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;


    char *drefSize = referenceNBytes(BOX_HEADER_HALF_SIZE, boxData, &bytesRead);
    char *drefType = referenceNBytes(BOX_HEADER_HALF_SIZE, boxData, &bytesRead);
    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *numberOfEntriesInt = charToUnsignedInt(numberOfEntries);


    Node *dataReferenceHeadNode = (Node*) malloc(sizeof(Node));
    Node *dataReferenceCurrentNode = dataReferenceHeadNode;
    for (int i = 0; i < *numberOfEntriesInt; i++) { 
        dataReference *dataReferenceChunk = (dataReference*) malloc(sizeof(dataReference)); 
        
        char *dataReferenceChunkSize = referenceNBytes(BOX_HEADER_HALF_SIZE, boxData, &bytesRead); 
        dataReferenceChunk->size = charToUnsignedInt(dataReferenceChunkSize);
        dataReferenceChunk->type = referenceNBytes(BOX_HEADER_HALF_SIZE, boxData, &bytesRead);
        dataReferenceChunk->version = referenceNBytes(VERSION_SIZE, boxData, &bytesRead);
        dataReferenceChunk->flags = referenceNBytes(FLAG_SIZE, boxData, &bytesRead);

        unsigned int dataReferenceChunkDataSize = *(dataReferenceChunk->size) - BOX_HEADER_SIZE - VERSION_SIZE - FLAG_SIZE;
        dataReferenceChunk->data = referenceNBytes(dataReferenceChunkDataSize, boxData, &bytesRead);

        printNBytes(dataReferenceChunk->type, BOX_HEADER_HALF_SIZE, "", "\n");

        // UPDATE TO USE NEW LINKED LIST STRUCT
        /* dataReferenceCurrentNode->currentBox = dataReferenceChunk;
        dataReferenceCurrentNode = dataReferenceCurrentNode->nextBoxNode;
        dataReferenceCurrentNode = NULL; */
    }
}


void vmhdParseBox(box *vmhdBox) { 
    unsigned int boxSize = *(vmhdBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = vmhdBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *graphicsMode = referenceNBytes(2, boxData, &bytesRead);
    char *opcolorOne = referenceNBytes(2, boxData, &bytesRead); //Red
    char *opcolorTwo = referenceNBytes(2, boxData, &bytesRead); //Green
    char *opcolorThree = referenceNBytes(2, boxData, &bytesRead); //Blue
}

/**
 * @brief for now returns the sub type to indicate video track
 * @param hdlrBox 
 * @return 
 */
char *hdlrParseBox(box *hdlrBox) {
    // IMPORTANT BOX FOR IDENTIFYING IF THE CURRENT TRAK IS VIDEO OR AUDIO
    unsigned int boxSize = *(hdlrBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = hdlrBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *componentType = copyNBytes(4, boxData, &bytesRead); // test files show that this is empty
    //printNBytes(componentType, 4, "type: ", "\n");
    char *componentSubtype = referenceNBytes(4, boxData, &bytesRead); // check if type is vide !IMPORTANT
    //printNBytes(componentSubtype, 4, "sub type: ", "\n");

    char *componentManufacturer = referenceNBytes(4, boxData, &bytesRead);
    char *componentFlags = referenceNBytes(4, boxData, &bytesRead);
    char *componentFlagsMask = referenceNBytes(4, boxData, &bytesRead);
    
    int componentNameLength = boxDataSize - bytesRead;
    char *componentName = referenceNBytes(componentNameLength, boxData, &bytesRead);
    //printNBytes(componentName, componentNameLength, "name: ", "\n");

    /* printf("%d %d\n", bytesRead, boxDataSize);
    if (bytesRead == boxDataSize) {
        printf("read all\n");
    } else {
        printf("no\n");
    } */ 

    return componentSubtype;

}






void elstParseBox(box *elstBox, MPEG_Data *videoData) { 
    unsigned int boxSize = *(elstBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = elstBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *numberOfEntriesInt = charToUnsignedInt(numberOfEntries);
    printf("number of edit list entries: %u\n", *numberOfEntriesInt);

    char *trackDuration = referenceNBytes(4, boxData, &bytesRead);
    char *mediaTime = referenceNBytes(4, boxData, &bytesRead);
    char *mediaRate = referenceNBytes(4, boxData, &bytesRead);

    // there can be multiple table entries in for example the video stops in
    // the middle of the movie and then continues later on
    elstTableEntry *elstTable = (elstTableEntry*) calloc(*numberOfEntries, sizeof(elstTableEntry*));
    


    unsigned int *trackDurationInt = charToUnsignedInt(trackDuration);
    unsigned int *mediaTimeInt = charToUnsignedInt(mediaTime);
    unsigned int *mediaRateInt = charToUnsignedInt(mediaRate);
    printf("duration: %u\n", *trackDurationInt);
    printf("time: %u\n", *mediaTimeInt);
    printf("rate: %u\n", *mediaRateInt);

    // temp error message
    if (*numberOfEntriesInt > 1) {
        printf("more than one elst table entry, parse rest");
    }

}


void edtsParseBox(box *edtsBox, MPEG_Data *videoData) {
    //Note: If the edit atom or the edit list atom is missing, you can assume that the entire media is used by the track.
    unsigned int boxSize = *(edtsBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = edtsBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    box *elstBox = parseSingleNestedChildBox(boxData, &bytesRead);
    elstParseBox(elstBox, videoData);
}


void tkhdParseBox(box *trakBox, MPEG_Data *videoData) {
    unsigned int boxSize = *(trakBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = trakBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *creationTime = referenceNBytes(4, boxData, &bytesRead);
    char *modificationTime = referenceNBytes(4, boxData, &bytesRead);
    char *trackId = referenceNBytes(4, boxData, &bytesRead);
    char *reservedOne = referenceNBytes(4, boxData, &bytesRead);

    // Start Of Used Values //
    char *duration = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *durationInt = charToUnsignedInt(duration);
    videoData->videoTrackDuration = durationInt;
    // End Of Used Values //
    printf("duration: %d\n", *durationInt);


    char *reservedTwo = referenceNBytes(8, boxData, &bytesRead);
    char *layer = referenceNBytes(2, boxData, &bytesRead);
    char *alternateGroup = referenceNBytes(2, boxData, &bytesRead);
    char *volume = referenceNBytes(2, boxData, &bytesRead);
    char *reservedThree = referenceNBytes(2, boxData, &bytesRead);
    char *matrixStructure = referenceNBytes(36, boxData, &bytesRead);
    //printNBytes(matrixStructure, 36, "matrix: ", "\n");
    
    
    char *trackWidth = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *trackWidthInt = charToUnsignedInt(trackWidth);
    char *trackHeight = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *trackHeightInt = charToUnsignedInt(trackHeight);
    printf("width: %d height: %d\n", *trackWidthInt, *trackHeightInt);



    printf("%d %d\n", bytesRead, boxDataSize);
    if (bytesRead == boxDataSize) {
        printf("read all\n");
    } else {
        printf("no\n");
    } 
}
// use

/**
 *  reads a mvhd box
 *  @param *mvhdBox:    a pointer to an mvhd box struct
*/
void mvhdParseBox(box *mvhdBox, MPEG_Data *videoData) { 

    unsigned int boxSize = *(mvhdBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = mvhdBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *creationTime = referenceNBytes(4, boxData, &bytesRead);
    char *modificationTime = referenceNBytes(4, boxData, &bytesRead);
    
    
    // Start Of Used Values //
    char *timeScale = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *timeScaleInt = charToUnsignedInt(timeScale);
    videoData->timescale = timeScaleInt;

    char *duration = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *durationInt = charToUnsignedInt(duration);
    videoData->fullDuration = durationInt;
    // End Of Used Values //

    printf("timescale as int: %u\n", *timeScaleInt);
    printf("duration as int: %u\n", *durationInt);



    //printNBytes(duration, 4, "duration: ", ".\n");
    //printf("duration/timescale %lf\n", (*durationInt / *timeScaleInt) / 60.0);
    //printHexNBytes(duration, 4);
    
    char *preferredRate = referenceNBytes(4, boxData, &bytesRead);
    char *preferredVolume = referenceNBytes(2, boxData, &bytesRead);
    char *reserved = referenceNBytes(10, boxData, &bytesRead);
    char *matrixStructure = referenceNBytes(36, boxData, &bytesRead);
    char *previewTime = referenceNBytes(4, boxData, &bytesRead);
    char *previewDuration = referenceNBytes(4, boxData, &bytesRead);
    char *posterTime = referenceNBytes(4, boxData, &bytesRead);
    char *selectionTime = referenceNBytes(4, boxData, &bytesRead);
    char *selectionDuration = referenceNBytes(4, boxData, &bytesRead);
    char *currentTime = referenceNBytes(4, boxData, &bytesRead);
    char *nextTrackId = referenceNBytes(4, boxData, &bytesRead);

    printf("%d %d\n", bytesRead, boxDataSize);
    if (bytesRead == boxDataSize) {
        printf("read all\n");
    } else {
        printf("no\n");
    } 
}





/**
 *  reads a ftyp box's major brand, minor version, and compatible brands
 *  @param *ftypBox:    a pointer to a ftyp box struct
 */
void ftypParseBox(box *ftypBox) { 
    unsigned int boxSize = *(ftypBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = ftypBox->boxData;

    /*
        reading the ftyp fields
        majorBrand: 4 bytes the preferred brand or best use
        minorVersion: 4 bytes indicates the file format specification version
        compatibleBrands[]: an array of 4 byte compatible file formats
    */
    unsigned int bytesRead = 0;
    char *majorBrand = (char*) malloc(4);
    for (int i = 0; i < 4; i++) {
        majorBrand[i] = boxData[bytesRead];
        bytesRead += 1;
    }
    printNBytes(majorBrand, 4, "", "\n");

    char *minorVersion = (char*) malloc(4);
    for (int i = 0; i < 4; i++) {
        minorVersion[i] = boxData[bytesRead];
        bytesRead += 1;
    }
    printHexNBytes(minorVersion, 4);

    unsigned int compatibleBrandsSize = boxDataSize - (bytesRead + 1);
    char *compatibleBrands = (char*) malloc(compatibleBrandsSize);
    
    int compatibleBrandsByte = 0;
    while (bytesRead < boxDataSize) {
        compatibleBrands[compatibleBrandsByte] = boxData[bytesRead];
        bytesRead += 1;
        compatibleBrandsByte += 1;
    }

    for (int i = 0; i < compatibleBrandsSize; i+=4) {
        printNBytes(&compatibleBrands[i], 4, "", "\n");
    }
}









box *parseSingleNestedChildBox(char *boxData, unsigned int *bytesRead) {
    // parsing size and type from header
    char *childBoxHeaderSize = copyNBytes(BOX_HEADER_HALF_SIZE, boxData, bytesRead);
    char *childBoxHeaderType = copyNBytes(BOX_HEADER_HALF_SIZE, boxData, bytesRead);
    
    // converting size to int and freeing char array
    unsigned int *childFullBoxSize = charToUnsignedInt(childBoxHeaderSize);
    free(childBoxHeaderSize);
    
    // DEBUG printf("%u\t", *childFullBoxSize);
    // DEBUG printNBytes(childBoxHeaderType, 4,"", "\n");

    // reading body of childBox
    int childBoxDataSize = *childFullBoxSize - 8;
    char *childBoxData = copyNBytes(childBoxDataSize, boxData, bytesRead);
    
    // Creating a box struct to store current box
    box *currentChildBoxRead = (box*) malloc(sizeof(box));
    currentChildBoxRead->boxSize = childFullBoxSize;
    currentChildBoxRead->boxType = childBoxHeaderType;
    currentChildBoxRead->boxData = childBoxData;

    return currentChildBoxRead;
}

// give the following three linked list return types
// might not want to do that if final thing wants to append
// on one big linked list containing all parsed boxes


void parseNestedChildBoxes(char *boxData, unsigned int *bytesRead, unsigned int endIndex, linkedList *list) {
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
    unsigned int boxSize = *(parentBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = parentBox->boxData;

    unsigned int bytesRead = 0;
    parseNestedChildBoxes(boxData, &bytesRead, boxDataSize, list);
}



/**
 *  reads the top level boxes in an MPEG-4 binary file format
 *  @param fileName:    a character array (includes '\0'), a valid 
 *                      path to an mp4 file that exists
 */
void readMainBoxes(char fileName[], linkedList *list) { 
    FILE *video = fopen(fileName, "rb");
    size_t chunksread;
    while (!feof(video)) { 
        // reading and storing the size of a box
        char *headerSize = (char*) malloc(BOX_HEADER_HALF_SIZE);
        chunksread = fread(&headerSize[0], BOX_HEADER_HALF_SIZE, 1, video);
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
        char *headerType = (char*) malloc(BOX_HEADER_HALF_SIZE);
        chunksread = fread(&headerType[0], BOX_HEADER_HALF_SIZE, 1, video);
        
        // translating the headerSize binary into an integer
        unsigned int *fullBoxSize = charToUnsignedInt(headerSize);
        free(headerSize); // freeing the char malloc for header size
        
        
        // DEBUG printBits(sizeof(int), fullBoxSize);
        // DEBUG printf("%u\n", *fullBoxSize);
        // DEBUG printf("%s\n", headerType);
        /* 
            FOR ADDING SUPPORT FOR BIGGER FILE SIZES WHEN THE 
            fullBoxSize is equal to 1, there is a field after the 
            type in the header that gives 8 bytes to store size
            char *extendedHeaderSize = (char*) malloc(8); 
        */

       
        /*
            fullBoxSize includes the size of the header 
            (box header: 8 bytes {[size -> 4 bytes] [type -> 4 bytes]})
            needs to be subtracted from fullBoxSize before the body of the box is read
        */
        int boxDataSize = *fullBoxSize - 8;
        char *boxData = (char*) malloc(boxDataSize);
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
