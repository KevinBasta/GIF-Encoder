#ifndef MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>

    #include "headers/types.h"
    #include "headers/typesUtility.h"
    #include "headers/printUtility.h"
    #include "headers/bitUtility.h"
    #include "headers/linkedList.h"
    #include "headers/processMPEG-4.h"
#endif



/**
 * @brief converts a real time in a real time coordinate system
 * to media time in media time coordinate system
 * @param realTimeInSeconds     -   with the decimal being milliseconds
 * @param mediaTimeScale        -   used for conversion
 * @return the time in media timescale that corresponds to the real time
 */
u32 realTimeToMediaTime(u32 realTimeInSeconds, u32 mediaTimeScale) { 
    // can put the to seconds conversion here later once interface is decided
    // need to use milliseconds too.  ideally a web interface would allow for 
    // precise time input.

    // testing adding milliseconds
    // u32 convertedTime = ((double) realTimeInSeconds + 0.058) * mediaTimeScale;
    
    // it's hard to get the time for the exact last sample in a movie
    u32 convertedTime = realTimeInSeconds * mediaTimeScale;
    //printf("converted time: %d\n", convertedTime);
    return convertedTime;
}


/**
 * @brief 
 * @param mediaTime             -   time to search for
 * @param timeToSampleTable     -   table to search in
 * @return sampleNumber
 */
u32 mediaTimeToSampleNumber(u32 mediaTime, timeToSampleTableEntry **timeToSampleTable) { 
    // need to consider edge cases here
    u32 sampleNumber = 0; 
    u32 sampleTimeAccumulator = 0;

    i32 timeToSampleEntryNumber = 0;
    while (timeToSampleTable[timeToSampleEntryNumber] != NULL) { 
        u32 sampleCountInTableEntry = timeToSampleTable[timeToSampleEntryNumber]->sampleCount;
        u32 sampleDurationInTableEntry = timeToSampleTable[timeToSampleEntryNumber]->sampleDuration;
        
        
        for (u32 i = 0; i < sampleCountInTableEntry; i++) { 
            if (sampleTimeAccumulator + sampleDurationInTableEntry <= mediaTime){ 
                sampleNumber++;
                sampleTimeAccumulator += sampleDurationInTableEntry;
            } else { 
                //printf("samples total: %d\n", sampleCountInTableEntry);
                return sampleNumber;
            }
        }

        timeToSampleEntryNumber++;
    }

    return 0; // error return
}


/**
 * @brief to allow for default sample size check incase no sample size table present.
 * +1 is needed to convert from 0 indexed array to 1 indexed tables.
 * @param sampleNumber  -   the sample whose size is requested
 * @param videoData     -   for accessing sampleSize and sampleSizeTable
 * @return  the size corresponding to the sample number
 */
u32 sampleNumberToSampleSize(u32 sampleNumber, u32 sampleSizeDefault, sampleSizeTableEntry **sampleSizeTable) { 
    if (sampleSizeDefault != 0) { 
        return sampleSizeDefault;
    } 

    return sampleSizeTable[sampleNumber + 1]->size;
}


/**
 * @brief sample needs to be passed in since multiple sampleInfo fields are populated
 * @param sample                -   used to read sampleNumber and store chunkNumber, 
 * sampleIndexInChunkNumber, and sampleOffsetInChunkNumber
 * @param sampleToChunkTable    -   for getting chunkNumber
 * @param numberOfSamples       -   for calculating lastChunk
 * @param sampleSizeTable       -   for calculating sampleOffsetInChunk
 * @param sampleSizeDefault     -   for calculating sampleOffsetInChunk
 * @return the chunkNumber that the sampleNumber resides in
 */
u32 sampleNumberToChunkNumber(sampleInfo *sample, 
                                       sampleToChunkTableEntry **sampleToChunkTable, 
                                       u32 numberOfSamples, 
                                       sampleSizeTableEntry **sampleSizeTable, 
                                       u32 sampleSizeDefault) { 
    // Data read from sample
    u32 sampleNumber = sample->sampleNumber;
    
    // Data written to sample
    u32 chunkNumber = 0;
    u32 sampleIndexInChunk = 0;
    u32 sampleOffsetInChunk = 0;

    u32 totalSamples = 0;

    u32 i = 0;
    while (sampleToChunkTable[i] != NULL) { 
        u32 firstChunk = sampleToChunkTable[i]->firstChunk;
        u32 samplesPerChunk = sampleToChunkTable[i]->samplesPerChunk;
        
        // "last" refers to the last chunk in this range before next table entry
        u32 lastChunk; 

        if (sampleToChunkTable[i + 1] != NULL) { 
            lastChunk = sampleToChunkTable[i + 1]->firstChunk - 1;
        } else { 
            lastChunk = (numberOfSamples / samplesPerChunk);
        }

        u32 chunksInChunkRange = lastChunk - firstChunk + 1;
        u32 samplesInChunkRange = chunksInChunkRange * samplesPerChunk;
        // DEBUG printf("%d %d %d %d %d\n", lastChunk, firstChunk, samplesPerChunk, samplesInChunkRange, totalSamples);

        if (sampleNumber <= (samplesInChunkRange + totalSamples)) { 
            for (u32 j = firstChunk; j <= lastChunk; j++) { 
                // DEBUG printf("total samples loop: %d\n", totalSamples);
                chunkNumber++;
                
                if (sampleNumber <= (samplesPerChunk + totalSamples)) { 
                    // NOTE: may be replacable by an equation
                    for (u32 i = 0; i < samplesPerChunk; i++) { 
                        totalSamples += 1;
                        sampleIndexInChunk += 1;
                        if (sampleNumber <= totalSamples) { 
                            sample->sampleIndexInChunk = sampleIndexInChunk;
                            sample->sampleOffsetInChunk = sampleOffsetInChunk;
                            sample->chunkNumber = chunkNumber;
                            return chunkNumber;
                        } else { 
                            sampleOffsetInChunk = sampleNumberToSampleSize(totalSamples, sampleSizeDefault, sampleSizeTable);
                        }
                    }
                } else { 
                    totalSamples += samplesPerChunk;
                }
            }
        } else { 
            totalSamples += samplesInChunkRange;
            chunkNumber += chunksInChunkRange;
            // DEBUG printf("%d\n", chunkNumb);
        }
        
        i++;
    }

    // DEBUG printf("total samples: %d\n", totalSamples);
    sample->sampleIndexInChunk = sampleIndexInChunk;
    sample->sampleOffsetInChunk = sampleOffsetInChunk;
    sample->chunkNumber = chunkNumber;
    return 0; // error return
}


/**
 * @brief mainly to avoid forgetting +1 since the table is 1 indexed but the array is 0 indexed
 * @param chunkNumber           -   chunk to search for
 * @param chunkOffsetTable      -   table to search in
 * @return chunk offset relative to the start of the file. NOT relative to any box.
 */
u32 chunkNumberToChunkOffset(u32 chunkNumber, chunkOffsetTableEntry **chunkOffsetTable) { 
    return chunkOffsetTable[chunkNumber + 1]->offset;
}


// sampleSizeAndChunkOffsetToSampleOffsetInChunk
u32 getSampleOffsetInChunk(sampleInfo *sample, u32 sampleSizeDefault, sampleSizeTableEntry **sampleSizeTable) { 
    // for general case when not in array

    u32 offsetAccumulator = 0;
    u32 sampleNumber = sample->sampleNumber;

    for (u32 i = 1; i < sample->sampleIndexInChunk; i++) { 
        offsetAccumulator += sampleNumberToSampleSize(sampleNumber - i, sampleSizeDefault, sampleSizeTable);
    }

    return offsetAccumulator;
}


u32 offsetDataToSampleMdatOffset(u32 chunkOffset, u32 sampleOffsetInChunk, u32 mdatOffsetInFile) { 
    u32 sampleMdatOffset = chunkOffset + sampleOffsetInChunk - mdatOffsetInFile;
    return sampleMdatOffset;
}



// Interface for working with a sampleInfo struct //

/**
 * @brief dependant on sample realTime
 */
void sampleRealTimeToMediaTime(sampleInfo *sample, MPEG_Data *videoData) {
    sample->mediaTime = realTimeToMediaTime(sample->realTime, videoData->mdhdTimeScale);
}

/**
 * @brief dependant on sampleRealTimeToMediaTime
 */
void sampleMediaTimeToSampleNumber(sampleInfo *sample, MPEG_Data *videoData) { 
    sample->sampleNumber = mediaTimeToSampleNumber(sample->mediaTime, videoData->timeToSampleTable);
}

/**
 * @brief dependant on sampleMediaTimeToSampleNumber
 */
void sampleSampleNumberToChunkNumber(sampleInfo *sample, MPEG_Data *videoData) { 
    sampleNumberToChunkNumber(sample, videoData->sampleToChunkTable, videoData->numberOfSamples, 
                              videoData->sampleSizeTable, videoData->sampleSizeDefault);
}

/**
 * @brief dependant on sampleSampleNumberToChunkNumber
 */
void sampleChunkNumberToChunkOffset(sampleInfo *sample, MPEG_Data *videoData) { 
    sample->chunkOffset = chunkNumberToChunkOffset(sample->chunkNumber, videoData->chunkOffsetTable);
}

/**
 * @brief dependant on sampleMediaTimeToSampleNumber or just sampleNumber
 */
void sampleSampleNumberToSampleSize(sampleInfo *sample, MPEG_Data *videoData) { 
    sample->sampleSize = sampleNumberToSampleSize(sample->sampleNumber, videoData->sampleSizeDefault, videoData->sampleSizeTable);
}

/**
 * @brief dependant on sampleSampleNumberToChunkNumber
 */
void sampleOffsetDataToSampleMdatOffset(sampleInfo *sample, MPEG_Data *videoData) { 
    sample->sampleOffsetInMdat = offsetDataToSampleMdatOffset(sample->chunkOffset, sample->sampleOffsetInChunk, videoData->mdatOffsetInFile);
}




void getVideoDataRange(u32 startTime, u32 endTime, MPEG_Data *videoData) { 
    sampleInfo *startSample = sampleSearchByTime(startTime, videoData);
    sampleInfo *endSample = sampleSearchByTime(endTime, videoData);

    u32 sampleRange = endSample->sampleNumber - startSample->sampleNumber;
    sampleInfo **sampleRangeArray = (sampleInfo**) calloc(sampleRange + 1, sizeof(sampleInfo*));
    sampleRangeArray[sampleRange] = NULL;
    sampleRangeArray[0] = startSample;
    sampleRangeArray[sampleRange - 1] = endSample;

    u32 iterSampleNumber = startSample->sampleNumber;
    for (u32 i = 1; i < (sampleRange - 1); i++) {
        iterSampleNumber += 1;
        sampleRangeArray[i] = sampleSearchBySampleNumber(iterSampleNumber, videoData);
    }
}

sampleInfo *sampleSearchByTime(u32 time, MPEG_Data *videoData) { 
    sampleInfo *sample = (sampleInfo*) malloc(sizeof(sampleInfo));
    sample->realTime = time;
    sampleRealTimeToMediaTime(sample, videoData);
    sampleMediaTimeToSampleNumber(sample, videoData);
    sampleSampleNumberToChunkNumber(sample, videoData);
    sampleChunkNumberToChunkOffset(sample, videoData);
    sampleSampleNumberToSampleSize(sample, videoData);
    sampleOffsetDataToSampleMdatOffset(sample, videoData);

    printf("media time: %d\n", sample->mediaTime);
    printf("sample: %d\n", sample->sampleNumber);
    printf("chunk: %d\n", sample->chunkNumber);
    printf("chunk offset: %d\n", sample->chunkOffset);
    printf("sample size: %d\n", sample->sampleSize);
    printf("sample index in chunk: %d\n", sample->sampleIndexInChunk);
    printf("sample offset in chunk: %d\n", sample->sampleOffsetInChunk);
    printf("total samples: %d\n", videoData->numberOfSamples);

    return sample;
}

sampleInfo *sampleSearchBySampleNumber(u32 sampleNumber, MPEG_Data *videoData) { 
    sampleInfo *sample = (sampleInfo*) malloc(sizeof(sampleInfo));
    sample->sampleNumber = sampleNumber;

    sampleSampleNumberToChunkNumber(sample, videoData);
    sampleChunkNumberToChunkOffset(sample, videoData);
    sampleSampleNumberToSampleSize(sample, videoData);
    sampleOffsetDataToSampleMdatOffset(sample, videoData);

    return sample;
}




sampleInfo *keyFrameSearch(u32 time, MPEG_Data *videoData) { 
    sampleInfo *sample = (sampleInfo*) malloc(sizeof(sampleInfo));
    sample->realTime = time;

    sampleRealTimeToMediaTime(sample, videoData);
    sampleMediaTimeToSampleNumber(sample, videoData);
    u32 keyframe = binarySearch(sample->sampleNumber, videoData->syncSampleTable, videoData->syncSampleTableEntries);
    printf("keyframe: %d\n", keyframe);

    sampleSampleNumberToChunkNumber(sample, videoData);
    sampleChunkNumberToChunkOffset(sample, videoData);
    sampleSampleNumberToSampleSize(sample, videoData);
    sampleOffsetDataToSampleMdatOffset(sample, videoData);

    return sample;
}