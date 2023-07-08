#ifndef MPEG_HEAD
    #define MPEG_HEAD
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    //#include <string.h>
    #include <math.h>
    #include <time.h>

    #include "../headers/types.h"
    #include "../headers/typeUtility.h"
    #include "../headers/printUtility.h"
    #include "../headers/endianUtility.h"
    #include "../headers/bitUtility.h"
    #include "../headers/linkedList.h"
    #include "../headers/AVCUtility.h"

    #include "../headers/parseMPEG-4.h"
    #include "../headers/decodeMPEG-4.h"
    #include "../headers/decodeAVC.h"
    #include "../headers/processMPEG-4.h"
#endif


void parseAVCSample(sampleInfo *sampleData, MPEG_Data *videoData) { 
    sampleData->mdatPointer = &(videoData->mdatBox->boxData[sampleData->sampleOffsetInMdat]);
    
    u32 bytesRead = sampleData->sampleOffsetInMdat;
    u32 sampleSize = sampleData->sampleSize;
    u32 unitLengthFieldSize = videoData->lengthSizeMinus1 + 1;
    // DEBUG printf("offset: %d\n", sampleData->sampleOffsetInMdat);
    int i;

    for (i = 0; i < sampleSize - 4;) {
        printf("i: %d\n", i);
        // since NALUnitLength can be 1, 2, or 4 bytes, will just store it in 4 byte int
        u8 *NALUnitLength    = referenceNBytes(unitLengthFieldSize, videoData->mdatBox->boxData, &bytesRead);
        // DEBUG printHexNBytes(&(videoData->mdatBox->boxData[sampleData->sampleOffsetInMdat]), 4);
        printf("bytes read: %d\n", bytesRead - sampleData->sampleOffsetInMdat);
        
        // generalized is requied here, spesific functions won't work because the byteNumb would be assumed
        u32 NALUnitFullLengthInt = bigEndianCharToLittleEndianGeneralized(NALUnitLength, unitLengthFieldSize); 
        u32 NALUnitDataLengthInt = NALUnitFullLengthInt - unitLengthFieldSize;

        u8 *NALDataStream          = referenceNBytes(NALUnitDataLengthInt, videoData->mdatBox->boxData, &bytesRead);
        parseNALUnit(NALUnitDataLengthInt, NALDataStream);

        i = i + unitLengthFieldSize + NALUnitDataLengthInt;

        // DEBUG printf("sample size: %7d nal unit length: %10u bytes read: %10u \n", sampleSize, NALUnitLengthInt, bytesRead);
    }
    printf("final i: %d\n", i);
    printf("sample Size: %d\n", sampleSize);
    printf("bytes read: %d\n", bytesRead - sampleData->sampleOffsetInMdat);
}


void parseNALUnits(NAL_Data *nalData) { 
    
}


void parseNALUnit(u32 NALUnitDataLength, u8 *NALDataStream) { 
    NALUnitInfo *NALUnit = (NALUnitInfo*) malloc(sizeof(NALUnitInfo));

    u32 bytesRead = 0;
    u8 emulationPreventionByte = FALSE;

    u8 *zeroBitAndNALIdcAndUnitType = copyNBytes(1, NALDataStream, &bytesRead);
    u8 forbiddenZeroBit             = getNBits(0, 0, *zeroBitAndNALIdcAndUnitType);
    u8 NALRefIdc                    = getNBits(1, 2, *zeroBitAndNALIdcAndUnitType); 
    u8 NALUnitType                  = getNBits(3, 7, *zeroBitAndNALIdcAndUnitType);
    free(zeroBitAndNALIdcAndUnitType);
    //u32 test = bigEndianU8ArrToLittleEndianU32(zeroBitAndNALIdcAndUnitType);
    //printf("===========================%d\n", test);

    // DEBUG printf("nal ref id %d  nal unit type %d\n", NALRefIdc, NALUnitType);
    
    // very slow process
    // discard sample if emulation byte present?
    /* for (int i = 1; i < NALUnitDataLength; i++) { 
        if (i + 2 < NALUnitDataLength && bigEndianCharToLittleEndianGeneralized(checkNextNBytes(3, NALDataStream, bytesRead), 3) == 0x000003) { 
            emulationPreventionByte = TRUE;
            bytesRead += 2;
            i += 2;
            // DEBUG printBits(referenceNBytes(1, NALDataStream, &bytesRead), 1);
        } else { 
            bytesRead++;
        }
    } */
    
    NALUnit->NALRefIdc = NALRefIdc;
    NALUnit->NALUnitType = NALUnitType;
    NALUnit->NALUnitData = NALDataStream + 1;
    //printf("%X\n", NALDataStream);
    //printf("%X\n", NALDataStream + 1);
    
    if (emulationPreventionByte == TRUE) { 
        NALUnit->NALUnitDataLength = NALUnitDataLength - 1;
    } else { 
        NALUnit->NALUnitDataLength = NALUnitDataLength;
    }

    if (NALUnitType == 8) { 
        picParameterSetRbsp(NALUnitDataLength - 1, NALDataStream + 1);
    } else if (NALUnitType == 7) { 
        seqParameterSetRbsp(NALUnitDataLength - 1, NALDataStream + 1);
    } else {
        printf("nal unit type: %d\n", NALUnitType);
    }
}


void picParameterSetRbsp(u32 NALUnitDataLength, u8 *NALDataStream) { 
    u32 leadingZeroBits;

    u32 bitsRead = 0;
    u32 bytesRead = 0;

    u8 *data = NALDataStream;
    u32 dataLength = NALUnitDataLength;

    printf("===============\n");

    u32 picParameterSetId       = ue(data, &bitsRead, &bytesRead, dataLength);
    u32 seqParameterSetId       = ue(data, &bitsRead, &bytesRead, dataLength);
    u32 entropyCodingModeFlag   = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    u32 picOrderPresentFlag     = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    u32 numSliceGroupsMinus1    = ue(data, &bitsRead, &bytesRead, dataLength);
    
    printf("%d %d %d %d %d\n\n", picParameterSetId, 
                                seqParameterSetId, 
                                entropyCodingModeFlag, 
                                picOrderPresentFlag, 
                                numSliceGroupsMinus1);


    u32 *runLengthMinus1            = NULL;
    u32 *topLeft                    = NULL;
    u32 *bottomRight                = NULL;
    u32 sliceGroupChangeDirectionFlag;
    u32 sliceGroupChangeRateMinus1;
    u32 picSizeInMapUnitsMinus1;
    u32 *sliceGroupId               = NULL;
    if (numSliceGroupsMinus1 > 0) { 
        u32 sliceGroupMapType               = ue(data, &bitsRead, &bytesRead, dataLength);
        printf("slice group: %d\n", sliceGroupMapType);
        
        if (sliceGroupMapType == 0) 
        {
            runLengthMinus1                 = malloc(sizeof(u32) * (numSliceGroupsMinus1 + 1));
            
            for (u32 iGroup = 0; iGroup <= numSliceGroupsMinus1; iGroup++) { 
                runLengthMinus1[iGroup]     = ue(data, &bitsRead, &bytesRead, dataLength);
            }
        }
        else if (sliceGroupMapType == 2) 
        {
            topLeft                         = malloc(sizeof(u32) * numSliceGroupsMinus1);
            bottomRight                     = malloc(sizeof(u32) * numSliceGroupsMinus1);

            for (u32 iGroup = 0; iGroup < numSliceGroupsMinus1; iGroup++) { 
                topLeft[iGroup]             = ue(data, &bitsRead, &bytesRead, dataLength);
                bottomRight[iGroup]         = ue(data, &bitsRead, &bytesRead, dataLength);
            }
        }
        else if (sliceGroupMapType == 3 || sliceGroupMapType == 4 || sliceGroupMapType == 5) 
        {     
            sliceGroupChangeDirectionFlag   = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
            sliceGroupChangeRateMinus1      = ue(data, &bitsRead, &bytesRead, dataLength);
        }
        else if (sliceGroupMapType == 6)
        { 
            picSizeInMapUnitsMinus1         = ue(data, &bitsRead, &bytesRead, dataLength); 
            sliceGroupId                    = malloc(sizeof(u32) * (picSizeInMapUnitsMinus1 + 1));
            
            u32 sliceGroupIdLength          = ceil( log2( numSliceGroupsMinus1 + 1 ) );
            for (u32 i = 0; i <= picSizeInMapUnitsMinus1; i++) { 
                sliceGroupId[i]             = getUnsignedNBits(data, &bitsRead, &bytesRead, sliceGroupIdLength);
            }
        }
    }


    u32 numRefIdxl0ActiveMinus1                 = ue(data, &bitsRead, &bytesRead, dataLength);
    u32 numRefIdxl1ActiveMinus1                 = ue(data, &bitsRead, &bytesRead, dataLength);
    u32 weightedPredFlag                        = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    u32 weightedBipredIdc                       = getUnsignedNBits(data, &bitsRead, &bytesRead, 2);
    i32 picInitQpMinus26                        = se(data, &bitsRead, &bytesRead, dataLength);
    i32 picInitQsMinus26                        = se(data, &bitsRead, &bytesRead, dataLength);
    i32 chromaQpIndexOffset                     = se(data, &bitsRead, &bytesRead, dataLength);
    u32 deblockingFilterVariablesPresentFlag    = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    u32 constrainedIntraPredFlag                = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    u32 redundantPicCntPresentFlag              = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    u32 frameCroppingFlag                       = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);

    u32 frameCropLeftOffset;
    u32 frameCropRightOffset;
    u32 frameCropTopOffset;
    u32 frameCropBottomOffset;
    if (frameCroppingFlag) { 
        frameCropLeftOffset     = ue(data, &bitsRead, &bytesRead, dataLength);
        frameCropRightOffset    = ue(data, &bitsRead, &bytesRead, dataLength);
        frameCropTopOffset      = ue(data, &bitsRead, &bytesRead, dataLength);
        frameCropBottomOffset   = ue(data, &bitsRead, &bytesRead, dataLength);
    }

    rbspTrailingBits(data, &bitsRead, &bytesRead, dataLength);

    printf("BITS READ %d\n", bitsRead);
    printf("TOTAL BITS %d\n", (dataLength * 8));
    printBits(data, 30);
    printf("===============\n");


    // TEMP FREEING
    if (runLengthMinus1 != NULL)
        free(runLengthMinus1);

    if (topLeft != NULL)
        free(topLeft);

    if (bottomRight != NULL)
        free(bottomRight);

    if (sliceGroupId != NULL)
        free(sliceGroupId);
}

void seqParameterSetRbsp(u32 NALUnitDataLength, u8 *NALDataStream) { 
    u32 leadingZeroBits;

    u32 bitsRead = 0;
    u32 bytesRead = 0;

    u8 *data = NALDataStream;
    u32 dataLength = NALUnitDataLength;

    printf("===============\n");
    u32 profileIdc                          = getUnsignedNBits(data, &bitsRead, &bytesRead, 8);
    u32 levelIdc                            = getUnsignedNBits(data, &bitsRead, &bytesRead, 8);
    u32 moreThanOneSliceGroupAllowedFlag    = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    u32 arbitrarySliceOrderAllowedFlag      = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    u32 redundantPicturesAllowedFlag        = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);

    u32 seqParameterSetId                   = ue(data, &bitsRead, &bytesRead, dataLength);
    u32 log2MaxFrameNumMinus4               = ue(data, &bitsRead, &bytesRead, dataLength);
    u32 picOrderCntType                     = ue(data, &bitsRead, &bytesRead, dataLength);


    u32 log2MaxPicOrderCntLsbMinus4;
    u32 deltaPicOrderAlwaysZeroFlag;
    u32 offsetForNonRefPic;
    u32 offsetForTopToBottomField;
    u32 numRefFramesInPicOrderCntCycle;
    i32 *offsetForRefFrame = NULL;
    if (picOrderCntType == 0)
    { 
        log2MaxPicOrderCntLsbMinus4     = se(data, &bitsRead, &bytesRead, dataLength);
    } 
    else if (picOrderCntType == 1)
    { 
        deltaPicOrderAlwaysZeroFlag     = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
        offsetForNonRefPic              = se(data, &bitsRead, &bytesRead, dataLength);
        offsetForTopToBottomField       = se(data, &bitsRead, &bytesRead, dataLength);
        numRefFramesInPicOrderCntCycle  = ue(data, &bitsRead, &bytesRead, dataLength);

        offsetForRefFrame = (i32*) malloc(sizeof(i32) * numRefFramesInPicOrderCntCycle);
        for (u32 i = 0; i < numRefFramesInPicOrderCntCycle; i++) { 
            offsetForRefFrame[i] = se(data, &bitsRead, &bytesRead, dataLength);
        }
    }


    u32 numRefFrames                            = ue(data, &bitsRead, &bytesRead, dataLength); 
    u32 requiredFrameNumUpdateBehaviourFlag     = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    u32 picWidthInMbsMinus1                     = ue(data, &bitsRead, &bytesRead, dataLength); 
    u32 picHeightInMapUnitsMinus1               = ue(data, &bitsRead, &bytesRead, dataLength); 
    u32 frameMbsOnlyFlag                        = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);

    u32 mbAdaptiveFrameFieldFlag;
    if (!frameMbsOnlyFlag) { 
        mbAdaptiveFrameFieldFlag                = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    }

    u32 direct8x8InferenceFlag                  = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    u32 vuiParametersPresentFlag                = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);

    if (vuiParametersPresentFlag) { 
        //vui_parameters();
        printf("vui parameters present\n");
    }

    rbspTrailingBits(data, &bitsRead, &bytesRead, dataLength);

    printf("BITS READ %d\n", bitsRead);
    printf("TOTAL BITS %d\n", (dataLength * 8));
    printBits(data, 30);
    printf("===============\n");

    if (offsetForRefFrame != NULL)
        free(offsetForRefFrame);
}



void sliceHeader(u32 NALUnitDataLength, u8 *NALDataStream) { 
    u32 leadingZeroBits;

    u32 bitsRead = 0;
    u32 bytesRead = 0;

    u8 *data = NALDataStream;
    u32 dataLength = NALUnitDataLength;

    printf("===============\n");

    u32 firstMbInSlice      = ue(data, &bitsRead, &bytesRead, dataLength);
    u32 sliceType           = ue(data, &bitsRead, &bytesRead, dataLength);
    u32 picParameterSetId   = ue(data, &bitsRead, &bytesRead, dataLength);

    u32 frameNum;

}



u8 byteAligned(u32 *bitsRead) { 
    if ((*bitsRead % 8) == 7)
        return TRUE;

    return FALSE;
}

void rbspTrailingBits(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 numberOfBits) {
    u8 rbspStopOneBit           = getUnsignedNBits(data, bitsRead, bytesRead, 1);
    printf("RBSP STOP ONE BIT: %d\n", rbspStopOneBit);

    u32 debugTrailingBitCounter = 1;
    while (byteAligned(bitsRead) == FALSE) { 
        u8 rbspAlignmentZeroBit = getUnsignedNBits(data, bitsRead, bytesRead, 1);
        debugTrailingBitCounter++;
    }

    printf("TOTAL TRAILING BITS: %d\n", debugTrailingBitCounter);
}



u32 u(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 numberOfBits) { 
    return getUnsignedNBits(data, bitsRead, bytesRead, numberOfBits);
}

u32 getUnsignedNBits(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 numberOfBits) { 
    u32 preReferenceBitsOffset      = *bitsRead;
    u8 *bigEndianDataReference      = referenceNBits(numberOfBits, data, bitsRead, bytesRead);
    u32 postReferenceBitsOffset     = *bitsRead;
    /* printf("preReferenceBitsOffset %d\n", preReferenceBitsOffset);
    printf("postReferenceBitsOffset %d\n", postReferenceBitsOffset);
    printf("preReferenceBitsOffset+numberOfBits %d\n\n", preReferenceBitsOffset+numberOfBits); */

    u32 readBits                    = simpleBigEndianToLittleEndianBits(bigEndianDataReference, preReferenceBitsOffset, postReferenceBitsOffset-1, numberOfBits);
    return readBits;
}



u32 ue(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 dataLength) { 
    return getCodeNum(data, bitsRead, bytesRead, dataLength);
}

i32 se(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 dataLength) { 
    u32 codeNum = getCodeNum(data, bitsRead, bytesRead, dataLength);
    return (i32) (pow(-1, (codeNum + 1)) * ceil(codeNum / 2));
}

/**
 * @brief ue(v)
 * @param data 
 * @param bitsRead 
 * @param bytesRead 
 * @param dataLength 
 * @return 
 */
u32 getCodeNum(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 dataLength) { 
    u32 leadingZeroBits             = countBitsToFirstNonZero(data, bitsRead, bytesRead, dataLength);
    //printf("bits bytes 1: %d %d\n", *bitsRead, *bytesRead);

    u32 leadingBitsOffsetPlusTwo    = *bitsRead;
    u8 *bigEndianDataReference      = referenceNBits(leadingZeroBits, data, bitsRead, bytesRead);
    u32 postReferenceBitsOffset     = *bitsRead;
    //printf("bits bytes 2: %d %d\n", *bitsRead, *bytesRead);

    u32 readBits                    = simpleBigEndianToLittleEndianBits(bigEndianDataReference, leadingBitsOffsetPlusTwo, postReferenceBitsOffset-1, leadingZeroBits);
    /* printf("leadingBitsOffsetPlusTwo %d\n", leadingBitsOffsetPlusTwo);
    printf("postReferenceBitsOffset %d\n", postReferenceBitsOffset);
    printf("leadingBitsOffsetPlusTwo+leadingZeroBits %d\n", leadingBitsOffsetPlusTwo+leadingZeroBits); */

    /* printf("readBits: %d\n", readBits);
    printf("data: %x %x %x %x\n", bigEndianDataReference[(u32) floor(leadingBitsOffsetPlusTwo / 8.0)],
                                      bigEndianDataReference[(u32) floor(leadingBitsOffsetPlusTwo / 8.0) + 1],
                                      bigEndianDataReference[(u32) floor(leadingBitsOffsetPlusTwo / 8.0) + 2],
                                      bigEndianDataReference[(u32) floor(leadingBitsOffsetPlusTwo / 8.0) + 3]);
    printf("starting bit: %d\n", (leadingBitsOffsetPlusTwo % 8));
    printf("leadingZeroBits: %d\n", leadingZeroBits); */



    //printf("bits bytes 3: %d %d\n", *bitsRead, *bytesRead);
    
    //printBits(bigEndianDataReference, (u32) floor(leadingZeroBits / 8));
    //printf("leading Zeros: %d\n", leadingZeroBits);
    //printf("bits little endian: %d\n", readBits);

    return (u32) (pow(2, leadingZeroBits) - 1 + readBits);
}
