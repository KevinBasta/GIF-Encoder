#ifndef MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    //#include <string.h>

    #include "headers/types.h"
    #include "headers/printUtility.h"
    #include "headers/bitUtility.h"
    #include "headers/linkedList.h"
    #include "headers/parseMPEG-4.h"
    #include "headers/processMPEG-4.h"
#endif

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
            // gather other info into MPEG_Data then
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
    unsigned int boxDataSize = stsdBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = stsdBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);

    unsigned int *numberOfEntriesInt = charToUnsignedInt(numberOfEntries);
    // DEBUG printf("%d\n", *numberOfEntriesInt);
    
    /* linkedList sampleDescriptions;
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
        
        //    Unspecified 28 bytes of all 0 bits not mentioned in spesification
        //    the last 4 bytes contains some unknown non-zero bits
        
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

    } */
    

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


/**
 * @brief sample size. REQUIRED.
 * to get size of each sample in media
 * @note path: moov->trak->mdia->minf->stbl->stsz
 * @param stszBox       -   the box
 * @param videoData     -   to store the following three things: 
 * sampleSizeInt, sampleSizeTable, and numberOfEntriesInt.
 * sampleSizeInt: if 0 then indicates that sample size table contains an entry 
 * for each sample. if non-zero then indicated the size of all samples.
 * sampleSizeTable: a table with an entry for each sample indicating 
 * it's size in bytes.
 * numberOfEntriesInt: equivalent to number of samples in media trak.
 */
void stszParseBox(box *stszBox, MPEG_Data *videoData) { //sample size required
    unsigned int boxDataSize = stszBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = stszBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);

    char *sampleSize = referenceNBytes(4, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    
    // If all the samples are the same size, this field contains that size value. 
    // If this field is set to 0, then the samples have different sizes, and those 
    // sizes are stored in the sample size table.
    unsigned int sampleSizeInt = bigEndianCharToLittleEndianUnsignedInt(sampleSize);

    // equivalent to the number of samples in the media
    unsigned int numberOfEntriesInt = bigEndianCharToLittleEndianUnsignedInt(numberOfEntries);

    //printf("%d\n", sampleSizeInt);
    //printf("%d\n", numberOfEntriesInt);
    
    sampleSizeTableEntry **sampleSizeTable = (sampleSizeTableEntry**) calloc(numberOfEntriesInt + 1, sizeof(sampleSizeTableEntry*));
    sampleSizeTable[numberOfEntriesInt] = NULL;

    for (int i = 0; i < numberOfEntriesInt; i++) { 
        char *size = referenceNBytes(4, boxData, &bytesRead);
        unsigned int sizeInt = bigEndianCharToLittleEndianUnsignedInt(size);

        sampleSizeTableEntry *sampleSizeEntry = (sampleSizeTableEntry*) malloc(sizeof(sampleSizeTableEntry));
        sampleSizeEntry->size = sizeInt;

        sampleSizeTable[i] = sampleSizeEntry;
        //printf("%d: %u\n", i + 1, sizeInt);
    }

    videoData->sampleSizeDefault = sampleSizeInt;
    videoData->sampleSizeTable = sampleSizeTable;
    videoData->numberOfSamples = numberOfEntriesInt;
}

/**
 * @brief sample to chunk. REQUIRED.
 * A chunk contains one or more samples. 
 * Contains a table that maps samples to their chunks.
 * Each table entry corresponds to a set of consecutive chunks, each of which contains the same number of samples. 
 * Furthermore, each of the samples in these chunks must use the same sample description. Whenever the number of 
 * samples per chunk or the sample description changes, you must create a new table entry. If all the chunks have 
 * the same number of samples per chunk and use the same sample description, this table has one entry.
 * @note path: moov->trak->mdia->minf->stbl->stsc
 * @param stscBox       -   the box
 * @param *videoData    -   to store sample-to-chunk table
 */
void stscParseBox(box *stscBox, MPEG_Data *videoData) { //sample to chunk required
    unsigned int boxDataSize = stscBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = stscBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int numberOfEntriesInt = bigEndianCharToLittleEndianUnsignedInt(numberOfEntries);
    // DEBUG printf("%d\n", *numberOfEntriesInt);

    sampleToChunkTableEntry **sampleToChunkTable = (sampleToChunkTableEntry**) calloc(numberOfEntriesInt + 1, sizeof(sampleToChunkTableEntry*));
    sampleToChunkTable[numberOfEntriesInt] = NULL;

    for (int i = 0; i < numberOfEntriesInt; i++) { 
        // first chunk refers to the index number of the chunk 
        // identifying a range from this first index number to the 
        // next table entry's first chunk index value, or the end of the file
        // calculated by the number of samples per chunk
        char *firstChunk = referenceNBytes(4, boxData, &bytesRead); 
        char *samplesPerChunk = referenceNBytes(4, boxData, &bytesRead); 
        char *sampleDescriptionId = referenceNBytes(4, boxData, &bytesRead);

        unsigned int firstChunkInt = bigEndianCharToLittleEndianUnsignedInt(firstChunk); 
        unsigned int samplesPerChunkInt = bigEndianCharToLittleEndianUnsignedInt(samplesPerChunk); 
        unsigned int sampleDescriptionIdInt = bigEndianCharToLittleEndianUnsignedInt(sampleDescriptionId); 
        
        sampleToChunkTableEntry *sampleToChunkEntry = (sampleToChunkTableEntry*) malloc(sizeof(sampleToChunkTableEntry));
        sampleToChunkEntry->firstChunk = firstChunkInt;
        sampleToChunkEntry->samplesPerChunk = samplesPerChunkInt;
        sampleToChunkEntry->sampleDescriptionId = sampleDescriptionIdInt;

        sampleToChunkTable[i] = sampleToChunkEntry;

        // printf("%d \t\t %d \t\t %d \n", firstChunkInt, samplesPerChunkInt, sampleDescriptionIdInt);
    }

    videoData->sampleToChunkTable = sampleToChunkTable;
}

/**
 * @brief chunk offset. REQUIRED.
 * gives location of chunk data in media as file offsets, not offsets within
 * any atom. 
 * @note path: moov->trak->mdia->minf->stbl->stco
 * @param stcoBox       -   the box
 * @param videoData     -   to store chunk offset table
 */
void stcoParseBox(box *stcoBox, MPEG_Data *videoData) { //chunk offset required
    unsigned int boxDataSize = stcoBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = stcoBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int numberOfEntriesInt = bigEndianCharToLittleEndianUnsignedInt(numberOfEntries);
    // DEBUG printf("%d\n", numberOfEntriesInt);
    
    chunkOffsetTableEntry **chunkOffsetTable = (chunkOffsetTableEntry**) calloc(numberOfEntriesInt + 1, sizeof(chunkOffsetTableEntry*));
    chunkOffsetTable[numberOfEntriesInt] = NULL;

    for (int i = 0; i < numberOfEntriesInt; i++) {
        char *offset = referenceNBytes(4, boxData, &bytesRead);
        unsigned int offsetInt = bigEndianCharToLittleEndianUnsignedInt(offset);

        chunkOffsetTableEntry *chunkOffsetEntry = (chunkOffsetTableEntry*) malloc(sizeof(chunkOffsetTableEntry));
        chunkOffsetEntry->offset = offsetInt;

        chunkOffsetTable[i] = chunkOffsetEntry;
        // DEBUG printf("%d: %d\n", i, offsetInt);
    }

    videoData->chunkOffsetTable = chunkOffsetTable;
}

/**
 * @brief time to sample. 
 * stores duration information of media's samples.
 * the time-to-sample table can be used to map from media time to 
 * corresponding data sample.
 * @note path: moov->trak->mdia->minf->stbl->stts
 * @param sttsBox       -   the box
 * @param videoData     -   to store time-to-sample table
 */
void sttsParseBox(box *sttsBox, MPEG_Data *videoData) { //time to sample
    unsigned int boxDataSize = sttsBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = sttsBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int numberOfEntriesInt = bigEndianCharToLittleEndianUnsignedInt(numberOfEntries);
    // DEBUG printf("%d\n", *numberOfEntriesInt);

    // unsigned int numberOfSamples = 0; Done without accumulator in stsz

    timeToSampleTableEntry **timeToSampleTable = (timeToSampleTableEntry**) calloc(numberOfEntriesInt + 1, sizeof(timeToSampleTableEntry*));
    timeToSampleTable[numberOfEntriesInt] = NULL;
        
    for (int i = 0; i < numberOfEntriesInt; i++) { 
        char *sampleCount = referenceNBytes(4, boxData, &bytesRead);
        char *sampleDuration = referenceNBytes(4, boxData, &bytesRead);
        
        unsigned int sampleCountInt = bigEndianCharToLittleEndianUnsignedInt(sampleCount);
        unsigned int sampleDurationInt = bigEndianCharToLittleEndianUnsignedInt(sampleDuration);
        
        // numberOfSamples += sampleCountInt;

        timeToSampleTableEntry *timeToSampleEntry = (timeToSampleTableEntry*) malloc(sizeof(timeToSampleTableEntry));
        timeToSampleEntry->sampleCount = sampleCountInt; 
        timeToSampleEntry->sampleDuration = sampleDurationInt;

        timeToSampleTable[i] = timeToSampleEntry;
    }

    videoData->timeToSampleTable = timeToSampleTable;
    // videoData->numberOfSamples = numberOfSamples; 
    // DEBUG printf("%d %d\n", videoData->timeToSampleTable[0]->sampleCount, videoData->timeToSampleTable[0]->sampleDuration);
}

/**
 * @brief sync sample.
 * https://wiki.multimedia.cx/index.php/QuickTime_container#stss
 * @param stssBox 
 * @param videoData 
 */
void stssParseBox(box *stssBox, MPEG_Data *videoData) { //sync sample
    if (stssBox == NULL) { 
        videoData->syncSampleTable = NULL;
        return;
    }

    unsigned int boxDataSize = stssBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = stssBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int numberOfEntriesInt = bigEndianCharToLittleEndianUnsignedInt(numberOfEntries);
    // DEBUG printf("%d\n", numberOfEntriesInt);

    syncSampleTableEntry **syncSampleTable = (syncSampleTableEntry**) calloc(numberOfEntriesInt + 1, sizeof(syncSampleTableEntry*));
    syncSampleTable[numberOfEntriesInt] = NULL;

    for (int i = 0; i < numberOfEntriesInt; i++) { 
        char *entry = referenceNBytes(4, boxData, &bytesRead);
        unsigned int entryInt = bigEndianCharToLittleEndianUnsignedInt(entry);
        
        syncSampleTableEntry *syncSampleEntry = (syncSampleTableEntry*) malloc(sizeof(syncSampleTableEntry));
        syncSampleEntry->number = entryInt;

        syncSampleTable[i] = syncSampleEntry;

        printf("%d\n", entryInt);
    }

    videoData->syncSampleTable = syncSampleTable;
    videoData->syncSampleTableEntries = numberOfEntriesInt;
}

/**
 * @brief composition offset.
 * Video samples in encoded formats have a decode order and presentation order.
 * presentation order == composition order == display order. 
 * This box is used when there are out of order video samples.
 * If the decode and presentation orders are the same, this box will 
 * not be present.
 * If video samples are stored out of presenation order, this box 
 * contains the time of presentation as a delta on sample by sample basis.
 * DisplayTime = MediaTime + DisplayTimeDelta
 * @param cttsBox       -   the box
 * @param videoData     -   to store compositionOffsetTable
 */
void cttsParseBox(box *cttsBox, MPEG_Data *videoData) { //composition offset
    unsigned int boxDataSize = cttsBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = cttsBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int numberOfEntriesInt = bigEndianCharToLittleEndianUnsignedInt(numberOfEntries);
    // printf("%d\n", numberOfEntriesInt);
    // printf("Sample Count \t\t Composition Offset\n");

    compositionOffsetTableEntry **compositionOffsetTable = (compositionOffsetTableEntry**) calloc(numberOfEntriesInt + 1, sizeof(compositionOffsetTableEntry*));
    compositionOffsetTable[numberOfEntriesInt] = NULL;
    
    for (int i = 0; i < numberOfEntriesInt; i++) { 
        char *sampleCount = referenceNBytes(4, boxData, &bytesRead);
        char *compositionOffset = referenceNBytes(4, boxData, &bytesRead);
        
        unsigned int sampleCountInt = bigEndianCharToLittleEndianUnsignedInt(sampleCount);
        int compositionOffsetInt = bigEndianCharToLittleEndianInt(compositionOffset);

        compositionOffsetTableEntry *compositionOffsetEntry = (compositionOffsetTableEntry*) malloc(sizeof(compositionOffsetTableEntry));
        compositionOffsetEntry->sampleCount = sampleCountInt;
        compositionOffsetEntry->compositionOffset = compositionOffsetInt;

        compositionOffsetTable[i] = compositionOffsetEntry;
        
        // printf("%d \t\t %d\n", sampleCountInt, compositionOffsetInt);
    }

    videoData->compositionOffsetTable = compositionOffsetTable;
}

void videoMediaBox() { 

}



/* void stsdParseBox(box *stsdBox) { 
    unsigned int boxSize = *(stsdBox->boxSize);
    unsigned int boxDataSize = boxSize - BOX_HEADER_SIZE;
    char *boxData = stsdBox->boxData;

} */






/**
 * @brief theoretically supposed to contain instructions 
 * for how to access the media's data in the data references table.
 * @note path: moov->trak->mdia->minf->dinf->dref
 * @param drefBox       -   the box
 * @param *videoData    -   for storing the data reference table
 */
void drefParseBox(box *drefBox, MPEG_Data *videoData) { 
    unsigned int boxDataSize = drefBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = drefBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;
    
    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int numberOfEntriesInt = bigEndianCharToLittleEndianUnsignedInt(numberOfEntries);
    printf("entries: %d\n", numberOfEntriesInt);

    dataReferenceTableEntry **dataReferenceTable = (dataReferenceTableEntry**) calloc(numberOfEntriesInt + 1, sizeof(dataReferenceTableEntry*)); 
    dataReferenceTable[numberOfEntriesInt] = NULL;
    
    for (int i = 0; i < numberOfEntriesInt; i++) { 
        dataReferenceTableEntry *dataReferenceEntry = (dataReferenceTableEntry*) malloc(sizeof(dataReferenceTableEntry));
        char *dataReferenceSize = referenceNBytes(BOX_HEADER_HALF_SIZE, boxData, &bytesRead);
        dataReferenceEntry->size = bigEndianCharToLittleEndianUnsignedInt(dataReferenceSize);

        dataReferenceEntry->type = copyNBytes(BOX_HEADER_HALF_SIZE, boxData, &bytesRead);

        // flag is used to indicate that media's data is in same file
        dataReferenceEntry->version = copyNBytes(VERSION_SIZE, boxData, &bytesRead);
        dataReferenceEntry->flags = copyNBytes(FLAG_SIZE, boxData, &bytesRead);

        unsigned int dataReferenceDataSize = dataReferenceEntry->size - BOX_HEADER_SIZE - VERSION_SIZE - FLAG_SIZE;
        dataReferenceEntry->data = copyNBytes(dataReferenceDataSize, boxData, &bytesRead);

        
        dataReferenceTable[i] = dataReferenceEntry;
        
        /* printNBytes(dataReferenceEntry->type, BOX_HEADER_HALF_SIZE, "", "\n");
        printBits(dataReferenceEntry->data, dataReferenceDataSize);
        printBits(dataReferenceEntry->flags, 3); */
    }

    videoData->dataReferenceTable = dataReferenceTable;
}


/**
 * @brief parses it's child box, deref. helps in interpreting the 
 * media's data.
 * @note path: moov->trak->mdia->minf->dinf
 * @param dinfBox       -   the box
 * @param *videoData    -   to pass to dref 
 */
void dinfParseBox(box *dinfBox, MPEG_Data *videoData) {
    unsigned int boxDataSize = dinfBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = dinfBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    box *drefBox = parseSingleNestedChildBox(boxData, &bytesRead);
    drefParseBox(drefBox, videoData);
}


/**
 * @brief unused box
 * @note path: moov->trak->mdia->minf->vmhd
 * @param vmhdBox 
 */
void vmhdParseBox(box *vmhdBox) { 
    unsigned int boxDataSize = vmhdBox->boxSize - BOX_HEADER_SIZE;
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
 * @brief identifies if the current trak is Video or Audio
 * @note path: moov->trak->mdia->hdlr
 * @param hdlrBox   -   the box
 * @return the componentSubtype that determins if this box's parent trak atom is 
 * of type video or not. vide return descides if should parse this trak further.
 */
char *hdlrParseBox(box *hdlrBox) { 
    unsigned int boxDataSize = hdlrBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = hdlrBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);

    // Start Of Used Values //
    char *componentType = referenceNBytes(4, boxData, &bytesRead);
    char *componentSubtype = referenceNBytes(4, boxData, &bytesRead); // check if type is vide !IMPORTANT
    // End Of Used Values //
    printNBytes(componentType, 4, "type ", "\n");
    printNBytes(componentSubtype, 4, "sub type ", "\n");

    char *componentManufacturer = referenceNBytes(4, boxData, &bytesRead);
    char *componentFlags = referenceNBytes(4, boxData, &bytesRead);
    char *componentFlagsMask = referenceNBytes(4, boxData, &bytesRead);

    int componentNameLength = boxDataSize - bytesRead;
    char *componentName = referenceNBytes(componentNameLength, boxData, &bytesRead);

    return componentSubtype;
}


/**
 * @brief gives characteristics of specific media
 * @note path: moov->trak->mdia->mdhd
 * @param mdhdBox   -   the box
 * @param videoData -   struct to store VIDEO timescale and duration
 */
void mdhdParseBox(box *mdhdBox, MPEG_Data *videoData) { 
    unsigned int boxDataSize = mdhdBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = mdhdBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *creationTime = referenceNBytes(4, boxData, &bytesRead);
    char *modificationTime = referenceNBytes(4, boxData, &bytesRead);

    // Start Of Used Values //
    char *timeScale = referenceNBytes(4, boxData, &bytesRead);
    char *duration = referenceNBytes(4, boxData, &bytesRead);
    
    unsigned int timeScaleInt = bigEndianCharToLittleEndianUnsignedInt(timeScale);
    unsigned int durationInt = bigEndianCharToLittleEndianUnsignedInt(duration);
    
    videoData->mdhdTimeScale = timeScaleInt;
    videoData->mdhdDuration = durationInt;
    // End Of Used Values //
    printf("%d %d\n", timeScaleInt, durationInt);

    
    char *language = referenceNBytes(2, boxData, &bytesRead);
    char *quality = referenceNBytes(2, boxData, &bytesRead);

    //printBits(language, 2);
    //printBits(quality, 2);
}


/**
 * @brief parses elst table
 * @note path: moov->trak->edts->elst
 * @param elstBox   -   the box
 * @param videoData -   struct to store elstTable in
 */
void elstParseBox(box *elstBox, MPEG_Data *videoData) { 
    unsigned int boxDataSize = elstBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = elstBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *numberOfEntries = referenceNBytes(4, boxData, &bytesRead);
    unsigned int numberOfEntriesInt = bigEndianCharToLittleEndianUnsignedInt(numberOfEntries);
    printf("number of edit list entries: %u\n", numberOfEntriesInt);

    // there can be multiple table entries in for example the video stops in
    // the middle of the movie and then continues later on
    
    //table *elstTable = (table*) malloc(sizeof(table));

    elstTableEntry **elstTable = calloc(numberOfEntriesInt + 1, sizeof(elstTableEntry*));
    elstTable[numberOfEntriesInt] = NULL;

    for (int i = 0; i < numberOfEntriesInt; i++) { 
        char *trackDuration = referenceNBytes(4, boxData, &bytesRead);
        char *mediaTime = referenceNBytes(4, boxData, &bytesRead);
        char *mediaRate = referenceNBytes(4, boxData, &bytesRead);

        unsigned int trackDurationInt = bigEndianCharToLittleEndianUnsignedInt(trackDuration);
        int mediaTimeInt = bigEndianCharToLittleEndianInt(mediaTime);
        unsigned int mediaRateInt = bigEndianCharToLittleEndianUnsignedInt(mediaRate);
        printf("%d %d %d\n", trackDurationInt, mediaTimeInt, mediaRateInt);

        elstTableEntry *elstEntry = (elstTableEntry*) malloc(sizeof(elstTableEntry));
        elstEntry->trackDuration = trackDurationInt;
        elstEntry->mediaTime = mediaTimeInt;
        elstEntry->mediaRate = mediaRateInt;
        
        elstTable[i] = elstEntry;
    }
    
    videoData->elstTable = elstTable;


    /* { 
        int i = 0;
        while (elstTable[i] != NULL) { 
            printf("%d %d %d\n", *(elstTable[i]->trackDuration), *(elstTable[i]->mediaTime), *(elstTable[i]->mediaRate));
            i++;
        }
    } */
}


/**
 * @brief parses it's single elst child box
 * @note path: moov->trak->edts
 * @param edtsBox   -   the box
 * @param videoData -   just for passing to child
 */
void edtsParseBox(box *edtsBox, MPEG_Data *videoData) {
    //Note: If the edit atom or the edit list atom is missing, you can assume that the entire media is used by the track.
    unsigned int boxDataSize = edtsBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = edtsBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    box *elstBox = parseSingleNestedChildBox(boxData, &bytesRead);
    elstParseBox(elstBox, videoData);
}


/**
 * @brief gives duration of track in question. used to get duration of video.
 * @note path: moov->trak->tkhd
 * @param trakBox   -   the box
 * @param videoData -   struct to store TRACK duration
 */
void tkhdParseBox(box *trakBox, MPEG_Data *videoData) {
    unsigned int boxDataSize = trakBox->boxSize - BOX_HEADER_SIZE;
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
    unsigned int durationInt = bigEndianCharToLittleEndianUnsignedInt(duration);
    videoData->tkhdTrackDuration = durationInt;
    // End Of Used Values //
    printf("duration: %d\n", durationInt);


    char *reservedTwo = referenceNBytes(8, boxData, &bytesRead);
    char *layer = referenceNBytes(2, boxData, &bytesRead);
    char *alternateGroup = referenceNBytes(2, boxData, &bytesRead);
    char *volume = referenceNBytes(2, boxData, &bytesRead);
    char *reservedThree = referenceNBytes(2, boxData, &bytesRead);
    char *matrixStructure = referenceNBytes(36, boxData, &bytesRead);
    //printNBytes(matrixStructure, 36, "matrix: ", "\n");
    
    
    char *trackWidth = referenceNBytes(4, boxData, &bytesRead);
    //unsigned int *trackWidthInt = charToUnsignedInt(trackWidth);
    char *trackHeight = referenceNBytes(4, boxData, &bytesRead);
    //unsigned int *trackHeightInt = charToUnsignedInt(trackHeight);
    //printf("width: %d height: %d\n", *trackWidthInt, *trackHeightInt);



    printf("%d %d\n", bytesRead, boxDataSize);
    if (bytesRead == boxDataSize) {
        printf("read all\n");
    } else {
        printf("no\n");
    } 
}


/**
 * @brief gives a timescale and duration for the entire movie
 * @note path: moov->mvhd
 * @param mvhdBox   -   the box
 * @param videoData -   struct to store ENTIRE MOVIE timescale and duration
 */
void mvhdParseBox(box *mvhdBox, MPEG_Data *videoData) { 
    unsigned int boxDataSize = mvhdBox->boxSize - BOX_HEADER_SIZE;
    char *boxData = mvhdBox->boxData;

    unsigned int bytesRead;
    bytesRead = 0;

    char *version = referenceNBytes(1, boxData, &bytesRead);
    char *flags = referenceNBytes(3, boxData, &bytesRead);
    char *creationTime = referenceNBytes(4, boxData, &bytesRead);
    char *modificationTime = referenceNBytes(4, boxData, &bytesRead);
    
    
    // Start Of Used Values //
    char *timeScale = referenceNBytes(4, boxData, &bytesRead);
    unsigned int timeScaleInt = bigEndianCharToLittleEndianUnsignedInt(timeScale);
    videoData->mvhdTimeScale = timeScaleInt;

    char *duration = referenceNBytes(4, boxData, &bytesRead);
    unsigned int durationInt = bigEndianCharToLittleEndianUnsignedInt(duration);
    videoData->mvhdDuration = durationInt;
    // End Of Used Values //

    printf("timescale as int: %u\n", timeScaleInt);
    printf("duration as int: %u\n", durationInt);



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
    unsigned int boxDataSize = ftypBox->boxSize - BOX_HEADER_SIZE;
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
    unsigned int childFullBoxSize = bigEndianCharToLittleEndianUnsignedInt(childBoxHeaderSize);
    free(childBoxHeaderSize);
    
    // DEBUG printf("%u\t", *childFullBoxSize);
    // DEBUG printNBytes(childBoxHeaderType, 4,"", "\n");

    // reading body of childBox
    int childBoxDataSize = childFullBoxSize - 8;
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
    unsigned int boxDataSize = parentBox->boxSize - BOX_HEADER_SIZE;
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
        unsigned int fullBoxSize = bigEndianCharToLittleEndianUnsignedInt(headerSize);
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
        int boxDataSize = fullBoxSize - 8;
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
