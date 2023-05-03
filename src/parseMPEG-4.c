#ifndef COMMON_HEAD
    #define COMMON_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include "headers/printUtility.h"
    #include "headers/bitUtility.h"
    #define BOX_HEADER_SIZE 8
    #define BOX_HEADER_HALF_SIZE 4
    #define TRUE 1
    #define FALSE 0
#endif

#include "headers/linkedList.h"
#define VERSION_SIZE 1
#define FLAG_SIZE 3


void readMainBoxes(char fileName[], linkedList *list);
void parseChildBoxes(box *moovBox, linkedList *list);
void ftypParseBox(box *ftypBox);
void mvhdParseBox(box *mvhdBox);
void tkhdParseBox(box *trakBox);
void elstParseBox(box *elstBox);
void hdlrParseBox(box *mdiaBox);
void vmhdParseBox(box *vmhdBox);
void dinfParseBox(box *dinfBox);


int main(int argc, char **argv) { 
    linkedList *topBoxesLL = initLinkedList();
    // fopen taken in a relative path from executable location
    readMainBoxes("local_files/op.mp4", topBoxesLL);
    
    box *moovBox;
    box *mvhdBox;
    box *trakBox;
    Node *moovHeadNode;
    Node *trakHeadNode;

    
    printf("----------TOP LEVEL----------\n");
    printAllBoxesLinkedList(topBoxesLL);
    

    printf("----------MOOV LEVEL----------\n");
    moovBox = getBoxFromLinkedList(topBoxesLL, "moov");
    
    linkedList *moovLL = initLinkedList();
    parseChildBoxes(moovBox, moovLL);
    printAllBoxesLinkedList(moovLL);


    printf("----------TRAK LEVEL----------\n");
    trakBox = getBoxFromLinkedList(moovLL, "trak");

    linkedList *trakLL = initLinkedList();
    parseChildBoxes(trakBox, trakLL);
    printAllBoxesLinkedList(trakLL);


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


    printf("--------STBL CHILD LEVEL--------\n");
    box *stsd = getBoxFromLinkedList(stblLL, "stsd");
    stsdParseBox(stsd);

    /*
    printf("----------MINF LEVEL----------\n");
    box *dinf = getBoxFromLinkedList(minfHeadNode, "dinf");
    dinfParseBox(dinf);
    */

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
    printf("end of script\n");
    return 0;
}


//https://developer.apple.com/library/archive/documentation/QuickTime/QTFF/QTFFChap2/qtff2.html#//apple_ref/doc/uid/TP40000939-CH204-25680
//the following child atoms are required: sample description, sample size, sample to chunk, and chunk offset
//If the sync sample atom is not present, all samples are implicitly sync samples.



typedef struct sampleDescription { 
    unsigned int *size; 
    char *dataFormat;
    char *reserved;
    unsigned short dataReferenceIndex;
} sampleDescription;

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

    unsigned int *numberOfEntriesInt = charToInt(numberOfEntries);
    // DEBUG printf("%d\n", *numberOfEntriesInt);
    
    linkedList sampleDescriptions;
    for (int i = 0; i < *numberOfEntries; i++) { 
        
    }
    

}

void stszParseBox(box *stszBox) { //sample size required
    
}

void stscParseBox(box *stscBox) { //sample to chunk required

}

void stcoParseBox(box *stcoBox) { //chunk offset required

}

void sttsParseBox(box *sttsBox) { //time to sample

}

void stssParseBox(box *stssBox) { //sync sample

}

void cttsParseBox(box *cttsBox) { //composition offset

}

void videoMediaBox() { 

}



/* void stsdParseBox(box *stsdBox) { 
    unsigned int boxSize = *(stsdBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = stsdBox->boxData;

} */




typedef struct dataReference { 
    unsigned int *size; 
    char *type;
    char *version;
    char *flags;
    char *data;
} dataReference;

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
    unsigned int *numberOfEntriesInt = charToInt(numberOfEntries);

    Node *dataReferenceHeadNode = (Node*) malloc(sizeof(Node));
    Node *dataReferenceCurrentNode = dataReferenceHeadNode;
    for (int i = 0; i < *numberOfEntriesInt; i++) { 
        dataReference *dataReferenceChunk = (dataReference*) malloc(sizeof(dataReference)); 
        
        char *dataReferenceChunkSize = referenceNBytes(BOX_HEADER_HALF_SIZE, boxData, &bytesRead); 
        dataReferenceChunk->size = charToInt(dataReferenceChunkSize);
        dataReferenceChunk->type = referenceNBytes(BOX_HEADER_HALF_SIZE, boxData, &bytesRead);
        dataReferenceChunk->version = referenceNBytes(VERSION_SIZE, boxData, &bytesRead);
        dataReferenceChunk->flags = referenceNBytes(FLAG_SIZE, boxData, &bytesRead);

        unsigned int dataReferenceChunkDataSize = *(dataReferenceChunk->size) - BOX_HEADER_SIZE - VERSION_SIZE - FLAG_SIZE;
        dataReferenceChunk->data = referenceNBytes(dataReferenceChunkDataSize, boxData, &bytesRead);


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


void hdlrParseBox(box *hdlrBox) {
    unsigned int boxSize = *(hdlrBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = hdlrBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *componentType = copyNBytes(4, boxData, &bytesRead); // test files show that this is empty
    printNBytes(componentType, 4, "type: ", "\n");
    char *componentSubtype = referenceNBytes(4, boxData, &bytesRead); // check if type is vide !IMPORTANT
    printNBytes(componentSubtype, 4, "sub type: ", "\n");

    char *componentManufacturer = referenceNBytes(4, boxData, &bytesRead);
    char *componentFlags = referenceNBytes(4, boxData, &bytesRead);
    char *componentFlagsMask = referenceNBytes(4, boxData, &bytesRead);
    
    int componentNameLength = boxDataSize - bytesRead;
    char *componentName = referenceNBytes(componentNameLength, boxData, &bytesRead);
    printNBytes(componentName, componentNameLength, "name: ", "\n");

    printf("%d %d\n", bytesRead, boxDataSize);
    if (bytesRead == boxDataSize) {
        printf("read all\n");
    } else {
        printf("no\n");
    } 

}


void elstParseBox(box *elstBox) {
    //Note: If the edit atom or the edit list atom is missing, you can assume that the entire media is used by the track.
    unsigned int boxSize = *(elstBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = elstBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *numberOfEntriesInt = charToInt(numberOfEntries);
    printf("number of edit list entries: %u\n", *numberOfEntriesInt);
    
    // NEED TO STORE IN DATA STRUCTURE IF THERE ARE MULTIPLE
    char *trackDuration = referenceNBytes(4, boxData, &bytesRead);
    char *mediaTime = referenceNBytes(4, boxData, &bytesRead);
    char *mediaRate = referenceNBytes(4, boxData, &bytesRead);

    /* 
    for (int editListEntryNumber = 1; editListEntryNumber <= *numberOfEntries; editListEntryNumber++) { 
        // current elst only have one entry, will generalize to store multiple entries when needed
        free(trackDuration);
        free(mediaTime);
        free(mediaRate);
        trackDuration = 
        mediaTime = referenceNBytes(4, boxData, &bytesRead);
        mediaRate = referenceNBytes(4, boxData, &bytesRead);
    } */

    unsigned int *trackDurationInt = charToInt(trackDuration);
    unsigned int *mediaTimeInt = charToInt(mediaTime);
    unsigned int *mediaRateInt = charToInt(mediaRate);
    printf("duration: %u\n", *trackDurationInt);
    printf("time: %u\n", *mediaTimeInt);
    printf("rate: %u\n", *mediaRateInt);
}


void tkhdParseBox(box *trakBox) {
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
    char *duration = referenceNBytes(4, boxData, &bytesRead);
    char *reservedTwo = referenceNBytes(8, boxData, &bytesRead);
    char *layer = referenceNBytes(2, boxData, &bytesRead);
    char *alternateGroup = referenceNBytes(2, boxData, &bytesRead);
    char *volume = referenceNBytes(2, boxData, &bytesRead);
    char *reservedThree = referenceNBytes(2, boxData, &bytesRead);
    char *matrixStructure = referenceNBytes(36, boxData, &bytesRead);
    char *trackWidth = referenceNBytes(4, boxData, &bytesRead);
    char *trackHeight = referenceNBytes(4, boxData, &bytesRead);
    printNBytes(trackWidth, 4, "width:", "\n");
    printf("width %u\n", charToInt(trackWidth));



    printf("%d %d\n", bytesRead, boxDataSize);
    if (bytesRead == boxDataSize) {
        printf("read all\n");
    } else {
        printf("no\n");
    } 
}


/**
 *  reads a mvhd box
 *  @param *mvhdBox:    a pointer to an mvhd box struct
*/
void mvhdParseBox(box *mvhdBox) { 
    unsigned int boxSize = *(mvhdBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = mvhdBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *creationTime = referenceNBytes(4, boxData, &bytesRead);
    char *modificationTime = referenceNBytes(4, boxData, &bytesRead);
    char *timeScale = referenceNBytes(4, boxData, &bytesRead);
    unsigned int *timeScaleInt = charToInt(timeScale);
    printf("timescale as int: %u\n", *timeScaleInt);
    
    char *duration = copyNBytes(4, boxData, &bytesRead);
    unsigned int *durationInt = charToInt(duration);
    printf("duration as int: %u\n", *durationInt);
    //printNBytes(duration, 4, "duration: ", ".\n");
    printf("duration/timescale %lf\n", (*durationInt / *timeScaleInt) / 60.0);
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


/**
 *  reads a moov box's first layer boxes
 *  @param *moovBox:    a pointer to a moov box struct
 */
void parseChildBoxes(box *parentBox, linkedList *list) {
    unsigned int boxSize = *(parentBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = parentBox->boxData;

    unsigned int bytesRead = 0;
    while (bytesRead < boxDataSize) { 
        // parsing size and type from header
        char *childBoxHeaderSize = (char*) malloc(BOX_HEADER_HALF_SIZE);
        for (int i = 0; i < BOX_HEADER_HALF_SIZE; i++) {
            childBoxHeaderSize[i] = boxData[bytesRead];
            bytesRead += 1;
        }

        char *childBoxHeaderType = (char*) malloc(BOX_HEADER_HALF_SIZE);
        for (int i = 0; i < BOX_HEADER_HALF_SIZE; i++) {
            childBoxHeaderType[i] = boxData[bytesRead];
            bytesRead += 1;
        }

        // converting size to int and freeing char array
        unsigned int *childFullBoxSize = charToInt(childBoxHeaderSize);
        free(childBoxHeaderSize);

        // DEBUG printf("%u\t", *childFullBoxSize);
        // DEBUG printNBytes(childBoxHeaderType, 4,"", "\n");

        // reading body of childBox
        int childBoxDataSize = *childFullBoxSize - 8;
        char *childBoxData = (char*) malloc(childBoxDataSize);
        for (int i = 0; i < childBoxDataSize; i++) {
            childBoxData[i] = boxData[bytesRead];
            bytesRead += 1;
        }

        // Creating a box struct to store current box
        box *currentChildBoxRead = (box*) malloc(sizeof(box));
        currentChildBoxRead->boxSize = childFullBoxSize;
        currentChildBoxRead->boxType = childBoxHeaderType;
        currentChildBoxRead->boxData = childBoxData;

        // Storing the current box in the current Node and allocating memory for the next box
        appendNodeLinkedList(list, currentChildBoxRead);
        if (bytesRead >= boxDataSize) {
            nullifyLastNodeLinkedList(list);
        }
    }
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
        unsigned int *fullBoxSize = charToInt(headerSize);
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
