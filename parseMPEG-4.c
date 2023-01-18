#ifndef COMMON_HEAD
    #define COMMON_HEAD
    #include <stdio.h>
    #include <stdlib.h>
#endif

#include "printUtility.c"

#define BOX_HEADER_SIZE 8
#define BOX_HEADER_HALF_SIZE 4
#define TRUE 1
#define FALSE 0

// general box struct
typedef struct box { 
    unsigned int *boxSize;
    char *boxType;
    char *boxData;
} box;

// box node struct
typedef struct Node { 
    void *currentBox;
    void *nextBoxNode;
} Node;


// utility  reading/parsing

int boxTypeEqual(char *boxType, char stringType[]);
unsigned int *charToInt(char *headerSize);
box *traverse(Node *headNode, int returnBox, char boxReturnType[]);
char *copyNBytes(int numberOfBytes, char *originalData, unsigned int *byteOffset);
char *referenceNBytes(int numberOfBytes, char *originalData, unsigned int *byteOffset);

Node *readMainBoxes(char fileName[]);
Node *parseChildBoxes(box *moovBox);
void ftypParseBox(box *ftypBox);
void mvhdParseBox(box *mvhdBox);
void tkhdParseBox(box *trakBox);
void elstParseBox(box *elstBox);


int main(int argc, char **argv) { 
    Node *headNode = readMainBoxes("op.mp4");
    
    box *moovBox;
    box *mvhdBox;
    box *trakBox;
    Node *moovHeadNode;
    Node *trakHeadNode;

    
    printf("----------TOP LEVEL----------\n");
    moovBox = traverse(headNode, TRUE, "moov");
    moovHeadNode = parseChildBoxes(moovBox);
    //mvhdBox = traverse(moovHeadNode, TRUE, "mvhd");
    //mvhdParseBox(mvhdBox);
        
    printf("----------MOOV LEVEL----------\n");
    trakBox = traverse(moovHeadNode, TRUE, "trak");
    trakHeadNode = parseChildBoxes(trakBox);
    printf("----------TRAK LEVEL----------\n");
    box *edts = traverse(trakHeadNode, TRUE, "edts");
    Node *edtsHeadNode = parseChildBoxes(edts);
    printf("----------EDTS LEVEL----------\n");
    box *elst = traverse(edtsHeadNode, TRUE, "elst");
    elstParseBox(elst);
   


    /* printf("----------TOP LEVEL----------\n");
    box *mdat = traverse(headNode, TRUE, "mdat");
    Node *mdatHeadNode = parseChildBoxes(mdat); */

    //tkhdParseBox(tkhd);

    //printf("%d\n", boxTypeEqual(headNode->currentBox->boxType, "ftyp"));
    //ftypParseBox(headNode->currentBox);
    
    printf("end of script\n");
    return 0;
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
 *  returns a char pointer to a new array. This new array contains n bytes
 *  of data from the originalData array 
 *  @param numberOfBytes:   amount of bytes to allocate for returned array
 *  @param *originalData:   pointer to an array
 *  @param *byteOffset:     the current array index of originalData
 *  @attention redundand memory usage if parent array presists
 *  @attention if using it might be benifitial to add a terminating char
 */
char *copyNBytes(int numberOfBytes, char *originalData, unsigned int *byteOffset) {
    char *infoCopy = (char*) malloc(numberOfBytes);
    for (int i = 0; i < numberOfBytes; i++) {
        infoCopy[i] = originalData[*byteOffset];
        *byteOffset += 1;
    }

    return infoCopy;
}


/**
 *  returns a char pointer from an array and increments byteOffset by the 
 *  size of the data that the char pointer refers to
 *  @param numberOfBytes:   amount of bytes to add to byteOffset
 *  @param *originalData:   pointer to an array
 *  @param *byteOffset:     the current array index of originalData
 *  @attention if parent array is freed data will be lost
 *  @attention will need to manually determine where each reference array ends
 */
char *referenceNBytes(int numberOfBytes, char *originalData, unsigned int *byteOffset) {
    char *infoReference = &(originalData[*byteOffset]);
    *byteOffset += numberOfBytes;

    // checking if referenced properly
    /* for (int i = 0; i < numberOfBytes; i++) { 
        assert(&(infoReference[i]) == &(originalData[(*byteOffset) - numberOfBytes + i]));
        assert(infoReference[i] == originalData[(*byteOffset) - numberOfBytes + i]);
    } */

    return infoReference;
}



/**
 *  traverses a linked list and returns a box if exsists
 *  @param *headNode:       the first element in the linked list
 *  @param returnBox:       TRUE or FALSE, allows for skiping the box type checking
 *  @param boxReturnType:   a character array that's compared against each box type
 *  @return a box struct pointer
 */
box *traverse(Node *headNode, int returnBox, char boxReturnType[]) { 
    box *boxToReturn = NULL;

    for (Node *traverseNode = headNode; traverseNode != NULL; traverseNode = traverseNode->nextBoxNode) {
        box *currentBoxPointer = (box*) traverseNode->currentBox;

        printNBytes(currentBoxPointer->boxType, 4, "box type: ", "\t");
        printf("box size: %10u\n", *(currentBoxPointer->boxSize));

        // && (boxToReturn == NULL) for taking the last match, will be based on which one is video
        if ((returnBox == TRUE) && (boxTypeEqual(currentBoxPointer->boxType, boxReturnType) == 1) && (boxToReturn == NULL)) {
            boxToReturn = traverseNode->currentBox;
        }

    }

    return boxToReturn;
}


/**
 *  reads a moov box's first layer boxes
 *  @param *moovBox:    a pointer to a moov box struct
 */
Node *parseChildBoxes(box *moovBox) {
    Node *headNode = (Node*) malloc(sizeof(Node));
    Node *currentNode = headNode;

    unsigned int boxSize = *(moovBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = moovBox->boxData;

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

        printf("%u\t", *childFullBoxSize);
        printNBytes(childBoxHeaderType, 4,"", "\n");

        // reading body of childBox
        int childBoxDataSize = *childFullBoxSize - 8;
        char *childBoxBody = (char*) malloc(childBoxDataSize);
        for (int i = 0; i < childBoxDataSize; i++) {
            childBoxBody[i] = boxData[bytesRead];
            bytesRead += 1;
        }

         // Creating a box struct to store current box
        box *currentChildBoxRead = (box*) malloc(sizeof(box));
        currentChildBoxRead->boxSize = childFullBoxSize;
        currentChildBoxRead->boxType = childBoxHeaderType;
        currentChildBoxRead->boxData = childBoxBody;

        // Storing the current box in the current Node and allocating memory for the next box
        currentNode->currentBox = currentChildBoxRead;
        if (bytesRead < boxDataSize) {
            currentNode->nextBoxNode = (Node*) malloc(sizeof(Node));
            currentNode = currentNode->nextBoxNode;
        } else { 
            currentNode->nextBoxNode = NULL;
        }
    }

    return headNode;
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
 *  reads the top level boxes in an MPEG-4 binary file format
 *  @param fileName:    a character array (includes '\0'), a valid 
 *                      path to an mp4 file that exists
 */
Node *readMainBoxes(char fileName[]) { 
    Node *headNode = (Node*) malloc(sizeof(Node));
    Node *currentNode = headNode;
    Node *lastNode;
    
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
            free(currentNode);
            lastNode->nextBoxNode = NULL;
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
        char *boxBody = (char*) malloc(boxDataSize);
        chunksread = fread(boxBody, boxDataSize, 1, video);
        

        // Creating a box struct to store current box
        box *currentBoxRead = (box*) malloc(sizeof(box));
        currentBoxRead->boxSize = fullBoxSize;
        currentBoxRead->boxType = headerType;
        currentBoxRead->boxData = boxBody;

        /*
            Storing the current box in the current Node and allocating memory for the next box
            setting the last node to the current node 
            (this is for setting it's nextBoxNode to null incase there is no next node)
        */
        currentNode->currentBox = currentBoxRead;
        currentNode->nextBoxNode = (Node*) malloc(sizeof(Node));
        lastNode = currentNode;
        currentNode = currentNode->nextBoxNode;
    }

    return headNode;
}


/**
 *  converts a 4 byte char array's binary to an unsigned int
 *  @param *integerAsCharArray:     the 4 byte character array
 */
unsigned int *charToInt(char *integerAsCharArray) { 
    unsigned int *fullBoxSize = (unsigned int*) malloc(sizeof(unsigned int));
    *fullBoxSize = 0;

    for (int headerByte = 0; headerByte < 4; headerByte++) {
        for (int bitInHeaderByte = 0; bitInHeaderByte < 8; bitInHeaderByte++) {
            /*
                currentBit (integerAsCharArray[headerByte] >> bitInHeaderByte) & 1
                shifts the desired bit to the lsb and masks it with the binary
                representation of 1

                bitOffset (((3-headerByte)*8) + bitInHeaderByte) maps the position
                of a bit in a byte to it's position in a 4 byte binary representation

                if the currentBit is 1, then will set that bit in the integer number
            */
            int currentBit = (integerAsCharArray[headerByte] >> bitInHeaderByte) & 1;
            int bitOffset = (((3-headerByte)*8) + bitInHeaderByte);
            // DEBUG printf("current bit: %d bit offset: %d\n", currentBit, bitOffset);

            if (currentBit == 1) {
                *fullBoxSize = *fullBoxSize | (currentBit << bitOffset);
            }
        }
    }

    return fullBoxSize;
}


/**
 *  compares the 4 byte box type to a string
 *  @param *boxType:    4 byte char array pointer
 *  @param stringType:  5 byte char array (includes '\0')
 */
int boxTypeEqual(char *boxType, char stringType[]) { 
    //char boxTypeStringArray[] = {boxType[0], boxType[1], boxType[2], boxType[3], '\0'};
    
    int isEqual = 1;
    for (int i = 0; i < 4; i++) {
        if (boxType[i] != stringType[i]) {
            isEqual = 0;
            break;
        }
    }

    return isEqual;
}