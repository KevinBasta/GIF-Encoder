#ifndef MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>

    #include "headers/types.h"
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
unsigned int realTimeToMediaTime(unsigned int realTimeInSeconds, unsigned int mediaTimeScale) { 
    // can put the to seconds conversion here later once interface is decided
    // need to use milliseconds too.  ideally a web interface would allow for 
    // precise time input.

    // testing adding milliseconds
    // unsigned int convertedTime = ((double) realTimeInSeconds + 0.058) * mediaTimeScale;
    
    // it's hard to get the time for the exact last sample in a movie
    unsigned int convertedTime = realTimeInSeconds * mediaTimeScale;
    //printf("converted time: %d\n", convertedTime);
    return convertedTime;
}


/**
 * @brief 
 * @param mediaTime 
 * @param timeToSampleTable 
 * @return 
 */
unsigned int mediaTimeToSampleNumber(unsigned int mediaTime, timeToSampleTableEntry **timeToSampleTable) { 
    // need to consider edge cases here
    unsigned int sampleNumber = 0; 
    unsigned int sampleTimeAccumulator = 0;

    int timeToSampleEntryNumber = 0;
    while (timeToSampleTable[timeToSampleEntryNumber] != NULL) { 
        unsigned int sampleCountInTableEntry = timeToSampleTable[timeToSampleEntryNumber]->sampleCount;
        unsigned int sampleDurationInTableEntry = timeToSampleTable[timeToSampleEntryNumber]->sampleDuration;
        
        
        for (int i = 0; i < sampleCountInTableEntry; i++) { 
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
 * @brief to allow for default sample size check incase no sample size table present
 * @param sampleNumber  -   the sample whose size is requested
 * @param videoData     -   for accessing sampleSize and sampleSizeTable
 * @return  the size corresponding to the sample number
 */
unsigned int sampleNumberToSampleSize(unsigned int sampleNumber, unsigned int sampleSizeDefault, sampleSizeTableEntry **sampleSizeTable) { 
    if (sampleSizeDefault != 0) { 
        return sampleSizeDefault;
    } 

    return sampleSizeTable[sampleNumber + 1]->size;
}


/**
 * @brief 
 * @param sample    -   used to read sampleNumber and store chunkNumber, sampleIndexInChunkNumber, and sampleOffsetInChunkNumber
 * @param sampleToChunkTable 
 * @param numberOfSamples 
 * @param sampleSizeTable 
 * @param sampleSizeDefault 
 * @return 
 */
unsigned int sampleNumberToChunkNumber(sampleInfo *sample, 
                                       sampleToChunkTableEntry **sampleToChunkTable, 
                                       unsigned int numberOfSamples, 
                                       sampleSizeTableEntry **sampleSizeTable, 
                                       unsigned int sampleSizeDefault) { 
    // Data read from sample
    unsigned int sampleNumber = sample->sampleNumber;
    
    // Data written to sample
    unsigned int chunkNumber = 0;
    unsigned int sampleIndexInChunk = 0;
    unsigned int sampleOffsetInChunk = 0;

    unsigned int totalSamples = 0;

    unsigned int i = 0;
    while (sampleToChunkTable[i] != NULL) { 
        unsigned int firstChunk = sampleToChunkTable[i]->firstChunk;
        unsigned int samplesPerChunk = sampleToChunkTable[i]->samplesPerChunk;
        
        // "last" refers to the last chunk in this range before next table entry
        unsigned int lastChunk; 

        if (sampleToChunkTable[i + 1] != NULL) { 
            lastChunk = sampleToChunkTable[i + 1]->firstChunk - 1;
        } else { 
            lastChunk = (numberOfSamples / samplesPerChunk);
        }

        unsigned int chunksInChunkRange = lastChunk - firstChunk + 1;
        unsigned int samplesInChunkRange = chunksInChunkRange * samplesPerChunk;
        // DEBUG printf("%d %d %d %d %d\n", lastChunk, firstChunk, samplesPerChunk, samplesInChunkRange, totalSamples);

        if (sampleNumber <= (samplesInChunkRange + totalSamples)) { 
            for (int j = firstChunk; j <= lastChunk; j++) { 
                // DEBUG printf("total samples loop: %d\n", totalSamples);
                chunkNumber++;
                
                if (sampleNumber <= (samplesPerChunk + totalSamples)) { 
                    // NOTE: may be replacable by an equation
                    for (int i = 0; i < samplesPerChunk; i++) { 
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



unsigned int chunkNumberToChunkOffset(unsigned int chunkNumber, chunkOffsetTableEntry **chunkOffsetTable) { 
    return chunkOffsetTable[chunkNumber + 1]->offset;
}






unsigned int getSampleOffsetInChunk(sampleInfo *sample, unsigned int sampleSizeDefault, sampleSizeTableEntry **sampleSizeTable) { 
    // for general case when not in array

    unsigned int offsetAccumulator = 0;
    unsigned int sampleNumber = sample->sampleNumber;

    for (int i = 1; i < sample->sampleIndexInChunk; i++) { 
        offsetAccumulator += sampleNumberToSampleSize(sampleNumber - i, sampleSizeDefault, sampleSizeTable);
    }

    return offsetAccumulator;
}



void sampleRealTimeToMediaTime(sampleInfo *sample, MPEG_Data *videoData) {
    sample->mediaTime = realTimeToMediaTime(sample->realTime, videoData->mdhdTimeScale);
}

void sampleMediaTimeToSampleNumber(sampleInfo *sample, MPEG_Data *videoData) { 
    sample->sampleNumber = mediaTimeToSampleNumber(sample->mediaTime, videoData->timeToSampleTable);
}

void sampleSampleNumberToChunkNumber(sampleInfo *sample, MPEG_Data *videoData) { 
    sampleNumberToChunkNumber(sample, videoData->sampleToChunkTable, videoData->numberOfSamples, 
                              videoData->sampleSizeTable, videoData->sampleSizeDefault);
}

void sampleChunkNumberToChunkOffset(sampleInfo *sample, MPEG_Data *videoData) { 
    sample->chunkOffset = chunkNumberToChunkOffset(sample->chunkNumber, videoData->chunkOffsetTable);
}

void sampleSampleNumberToSampleSize(sampleInfo *sample, MPEG_Data *videoData) { 
    sample->sampleSize = sampleNumberToSampleSize(sample->sampleNumber, videoData->sampleSizeDefault, videoData->sampleSizeTable);
}


void sampleSearch(unsigned int time, MPEG_Data *videoData) { 
    //printf("media duration: %d\n", *(videoData->mdhdDuration));
    //printf("media time scale: %d\n", *(videoData->mdhdTimeScale));


    // videoData can be passed as a whole to all of these functions to simplify interface, 
    // but allowing access to other data doesn't seem like a good idea

    sampleInfo *sample = (sampleInfo*) malloc(sizeof(sampleInfo));
    sample->realTime = time;
    sampleRealTimeToMediaTime(sample, videoData);
    sampleMediaTimeToSampleNumber(sample, videoData);
    sampleSampleNumberToChunkNumber(sample, videoData);
    sampleChunkNumberToChunkOffset(sample, videoData);
    sampleSampleNumberToSampleSize(sample, videoData);

    printf("media time: %d\n", sample->mediaTime);
    printf("sample: %d\n", sample->sampleNumber);
    printf("chunk: %d\n", sample->chunkNumber);
    printf("chunk offset: %d\n", sample->chunkOffset);
    printf("sample size: %d\n", sample->sampleSize);
    printf("sample index in chunk: %d\n", sample->sampleIndexInChunk);
    printf("sample offset in chunk: %d\n", sample->sampleOffsetInChunk);
    printf("total samples: %d\n", videoData->numberOfSamples);

}


void keyFrameSearch() { 
    
}