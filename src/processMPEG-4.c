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
u32 mediaTimeToDecodeSampleNumber(u32 mediaTime, timeToSampleTableCompressed *timeToSampleTable) { 
    // need to consider edge cases here
    u32 sampleNumber = 0; 
    u32 sampleTimeAccumulator = 0;

    u32 timeToSampleEntryNumber = 0;
    for (u32 i = 0; i < timeToSampleTable->totalEntries; i++) {
        u32 sampleCountInTableEntry = timeToSampleTable->sampleCountArr[i];
        u32 sampleDurationInTableEntry = timeToSampleTable->sampleDurationArr[i];
        
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


u32 mediaTimeToDisplaySampleNumber(u32 mediaTime, displayTimeToSampleTable **presentationTimeToSampleTable) { 

}


/**
 * @brief to allow for default sample size check incase no sample size table present.
 * +1 is needed to convert from 0 indexed array to 1 indexed tables.
 * @param sampleNumber  -   the sample whose size is requested
 * @param videoData     -   for accessing sampleSize and sampleSizeTable
 * @return  the size corresponding to the sample number
 */
u32 sampleNumberToSampleSize(u32 sampleNumber, sampleSizeTable *sampleSizeTable) { 
    if (sampleSizeTable->sampleSizeDefault != 0) { 
        return sampleSizeTable->sampleSizeDefault;
    } 

    return sampleSizeTable->sizeArr[sampleNumber + 1];
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
u32 sampleNumberToChunkNumber(sampleInfo *sample, sampleToChunkTable *sampleToChunkTable, sampleSizeTable *sampleSizeTable, u32 numberOfSamples) { 
    // Data read from sample
    u32 sampleNumber = sample->sampleNumber;
    
    // Data written to sample
    u32 chunkNumber = 0;
    u32 sampleIndexInChunk = 0;
    u32 sampleOffsetInChunk = 0;

    u32 totalSamples = 0;

    for (u32 i = 0; i < sampleToChunkTable->totalEntries; i++) {
        u32 firstChunk = sampleToChunkTable->firstChunkArr[i];
        u32 samplesPerChunk = sampleToChunkTable->samplesPerChunkArr[i];
        
        // "last" refers to the last chunk in this range before next table entry
        u32 lastChunk; 

        if (i + 1 <= sampleToChunkTable->totalEntries) { 
            lastChunk = sampleToChunkTable->firstChunkArr[i + 1] - 1;
        } else { 
            lastChunk = firstChunk + (numberOfSamples / samplesPerChunk);
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
                            sampleOffsetInChunk = sampleNumberToSampleSize(totalSamples, sampleSizeTable);
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
u32 chunkNumberToChunkOffset(u32 chunkNumber, chunkOffsetTable *chunkOffsetTable) { 
    return chunkOffsetTable->offsetArr[chunkNumber + 1];
}


// sampleSizeAndChunkOffsetToSampleOffsetInChunk
u32 getSampleOffsetInChunk(sampleInfo *sample, u32 sampleSizeDefault, sampleSizeTable *sampleSizeTable) { 
    // for general case when not in array

    u32 offsetAccumulator = 0;
    u32 sampleNumber = sample->sampleNumber;

    for (u32 i = 1; i < sample->sampleIndexInChunk; i++) { 
        offsetAccumulator += sampleNumberToSampleSize(sampleNumber - i, sampleSizeTable);
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
void sampleMediaTimeToDecodeSampleNumber(sampleInfo *sample, MPEG_Data *videoData) { 
    sample->sampleNumber = mediaTimeToDecodeSampleNumber(sample->mediaTime, videoData->timeToSampleTableCompressed);
}

void sampleMediaTimeToPlaybackSampleNumber(sampleInfo *sample, MPEG_Data *videoData) { 
    sample->sampleNumber;
}

/**
 * @brief dependant on sampleMediaTimeToSampleNumber
 */
void sampleSampleNumberToChunkNumber(sampleInfo *sample, MPEG_Data *videoData) { 
    sampleNumberToChunkNumber(sample, videoData->sampleToChunkTable, videoData->sampleSizeTable, videoData->numberOfSamples);
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
    sample->sampleSize = sampleNumberToSampleSize(sample->sampleNumber, videoData->sampleSizeTable);
}

/**
 * @brief dependant on sampleSampleNumberToChunkNumber
 */
void sampleOffsetDataToSampleMdatOffset(sampleInfo *sample, MPEG_Data *videoData) { 
    sample->sampleOffsetInMdat = offsetDataToSampleMdatOffset(sample->chunkOffset, sample->sampleOffsetInChunk, videoData->mdatOffsetInFile);
}


void expandAndBuildNewTable(MPEG_Data *videoData) { 
    // expand out time to sample
    // expand out composition offset
    u32 numberOfSamples = videoData->numberOfSamples;

    displayTimeToSampleTable *table = (displayTimeToSampleTable*) malloc(sizeof(displayTimeToSampleTable));
    u32 *displayTimeArr = (u32*) calloc(numberOfSamples, sizeof(u32));

    for (u32 i = 0; i < numberOfSamples; i++) { 
        displayTimeArr[i] = videoData->timeToSampleTable->sampleDeltaArr[i] + videoData->compositionOffsetTable->compositionOffsetArr[i];
    }

    table->displayTimeArr = displayTimeArr;
    table->totalEntries = numberOfSamples;

    // for each entry of both, add them, and store the result time in 
    // a struct along with the sample number

    // sort based on the result time
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
    sampleMediaTimeToDecodeSampleNumber(sample, videoData);
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
    sampleMediaTimeToDecodeSampleNumber(sample, videoData);
    u32 keyframe = binarySearch(sample->sampleNumber, videoData->syncSampleTable->sampleNumberArr, videoData->syncSampleTable->totalEntries);
    printf("keyframe: %d\n", keyframe);

    sampleSampleNumberToChunkNumber(sample, videoData);
    sampleChunkNumberToChunkOffset(sample, videoData);
    sampleSampleNumberToSampleSize(sample, videoData);
    sampleOffsetDataToSampleMdatOffset(sample, videoData);

    return sample;
}