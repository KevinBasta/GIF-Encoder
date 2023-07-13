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
    u32 unitLengthFieldSize = videoData->avcData->lengthSizeMinus1 + 1;
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


NALUnitInfo *parseNALUnit(u32 NALUnitDataLength, u8 *NALDataStream) {
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

    /* if (NALUnitType == 8) { 
        picParameterSetRbsp(NALUnitDataLength - 1, NALDataStream + 1);
    } else if (NALUnitType == 7) { 
        seqParameterSetRbsp(NALUnitDataLength - 1, NALDataStream + 1);
    } else {
        printf("nal unit type: %d\n", NALUnitType);
    } */

    return NALUnit;
}


picParameterSet *picParameterSetRbspDecode(u32 NALUnitDataLength, u8 *NALDataStream) { 
    NALUnitInfo *NALUnit = parseNALUnit(NALUnitDataLength, NALDataStream);
    
    u32 bitsRead = 0;
    u32 bytesRead = 0;

    u8 *data = NALUnit->NALUnitData;
    u32 dataLength = NALUnit->NALUnitDataLength;

    printf("===============\n");
    picParameterSet *pps = calloc(1, sizeof(picParameterSet));

    pps->picParameterSetId       = ue(data, &bitsRead, &bytesRead, dataLength);
    pps->seqParameterSetId       = ue(data, &bitsRead, &bytesRead, dataLength);
    pps->entropyCodingModeFlag   = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    pps->picOrderPresentFlag     = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    pps->numSliceGroupsMinus1    = ue(data, &bitsRead, &bytesRead, dataLength);
    
    printf("%d %d %d %d %d\n\n",pps->picParameterSetId, 
                                pps->seqParameterSetId, 
                                pps->entropyCodingModeFlag, 
                                pps->picOrderPresentFlag, 
                                pps->numSliceGroupsMinus1);

    
    pps->sliceGroupMapType;
    pps->runLengthMinus1            = NULL;
    pps->topLeft                    = NULL;
    pps->bottomRight                = NULL;
    pps->sliceGroupChangeDirectionFlag;
    pps->sliceGroupChangeRateMinus1;
    pps->picSizeInMapUnitsMinus1;
    pps->sliceGroupId               = NULL;

    if (pps->numSliceGroupsMinus1 > 0) { 
        pps->sliceGroupMapType = ue(data, &bitsRead, &bytesRead, dataLength);
        printf("slice group: %d\n", pps->sliceGroupMapType);
        
        if (pps->sliceGroupMapType == 0) 
        {

            pps->runLengthMinus1                = malloc(sizeof(u32) * (pps->numSliceGroupsMinus1 + 1));
            
            for (u32 iGroup = 0; iGroup <= pps->numSliceGroupsMinus1; iGroup++) { 
                pps->runLengthMinus1[iGroup]    = ue(data, &bitsRead, &bytesRead, dataLength);
            }

        } 
        else if (pps->sliceGroupMapType == 2) 
        {

            pps->topLeft                        = malloc(sizeof(u32) * pps->numSliceGroupsMinus1);
            pps->bottomRight                    = malloc(sizeof(u32) * pps->numSliceGroupsMinus1);

            for (u32 iGroup = 0; iGroup < pps->numSliceGroupsMinus1; iGroup++) { 
                pps->topLeft[iGroup]            = ue(data, &bitsRead, &bytesRead, dataLength);
                pps->bottomRight[iGroup]        = ue(data, &bitsRead, &bytesRead, dataLength);
            }

        }
        else if (pps->sliceGroupMapType == 3 || pps->sliceGroupMapType == 4 || pps->sliceGroupMapType == 5) 
        {   

            pps->sliceGroupChangeDirectionFlag  = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
            pps->sliceGroupChangeRateMinus1     = ue(data, &bitsRead, &bytesRead, dataLength);

        }
        else if (pps->sliceGroupMapType == 6)
        {

            pps->picSizeInMapUnitsMinus1        = ue(data, &bitsRead, &bytesRead, dataLength); 
            pps->sliceGroupId                   = malloc(sizeof(u32) * (pps->picSizeInMapUnitsMinus1 + 1));
            
            u32 sliceGroupIdLength              = ceil( log2( pps->numSliceGroupsMinus1 + 1 ) );
            for (u32 i = 0; i <= pps->picSizeInMapUnitsMinus1; i++) { 
                pps->sliceGroupId[i]            = getUnsignedNBits(data, &bitsRead, &bytesRead, sliceGroupIdLength);
            }

        }
    }

    pps->numRefIdxl0ActiveMinus1                 = ue(data, &bitsRead, &bytesRead, dataLength);
    pps->numRefIdxl1ActiveMinus1                 = ue(data, &bitsRead, &bytesRead, dataLength);
    pps->weightedPredFlag                        = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    pps->weightedBipredIdc                       = getUnsignedNBits(data, &bitsRead, &bytesRead, 2);
    pps->picInitQpMinus26                        = se(data, &bitsRead, &bytesRead, dataLength);
    pps->picInitQsMinus26                        = se(data, &bitsRead, &bytesRead, dataLength);
    pps->chromaQpIndexOffset                     = se(data, &bitsRead, &bytesRead, dataLength);
    pps->deblockingFilterVariablesPresentFlag    = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    pps->constrainedIntraPredFlag                = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    pps->redundantPicCntPresentFlag              = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    pps->frameCroppingFlag                       = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);

    if (pps->frameCroppingFlag) { 
        pps->frameCropLeftOffset     = ue(data, &bitsRead, &bytesRead, dataLength);
        pps->frameCropRightOffset    = ue(data, &bitsRead, &bytesRead, dataLength);
        pps->frameCropTopOffset      = ue(data, &bitsRead, &bytesRead, dataLength);
        pps->frameCropBottomOffset   = ue(data, &bitsRead, &bytesRead, dataLength);
    }

    rbspTrailingBits(data, &bitsRead, &bytesRead, dataLength);

    printf("BITS READ %d\n", bitsRead);
    printf("TOTAL BITS %d\n", (dataLength * 8));
    printBits(data, 30);
    printf("===============\n");


    return pps;

    // to move to freeing function
    if (pps->runLengthMinus1 != NULL)
        free(pps->runLengthMinus1);

    if (pps->topLeft != NULL)
        free(pps->topLeft);

    if (pps->bottomRight != NULL)
        free(pps->bottomRight);

    if (pps->sliceGroupId != NULL)
        free(pps->sliceGroupId);
    
    free(pps);
}



seqParameterSet *seqParameterSetRbspDecode(u32 NALUnitDataLength, u8 *NALDataStream) { 
    NALUnitInfo *NALUnit = parseNALUnit(NALUnitDataLength, NALDataStream);

    u32 bitsRead = 0;
    u32 bytesRead = 0;

    u8 *data = NALUnit->NALUnitData;
    u32 dataLength = NALUnit->NALUnitDataLength;

    printf("===============\n");
    seqParameterSet *sps = calloc(1, sizeof(seqParameterSet));

    sps->profileIdc                          = getUnsignedNBits(data, &bitsRead, &bytesRead, 8);
    sps->levelIdc                            = getUnsignedNBits(data, &bitsRead, &bytesRead, 8);
    sps->moreThanOneSliceGroupAllowedFlag    = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    sps->arbitrarySliceOrderAllowedFlag      = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    sps->redundantPicturesAllowedFlag        = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);

    sps->seqParameterSetId                   = ue(data, &bitsRead, &bytesRead, dataLength);
    sps->log2MaxFrameNumMinus4               = ue(data, &bitsRead, &bytesRead, dataLength);
    sps->picOrderCntType                     = ue(data, &bitsRead, &bytesRead, dataLength);


    sps->log2MaxPicOrderCntLsbMinus4;
    sps->deltaPicOrderAlwaysZeroFlag;
    sps->offsetForNonRefPic;
    sps->offsetForTopToBottomField;
    sps->numRefFramesInPicOrderCntCycle;
    sps->offsetForRefFrame = NULL;
    if (sps->picOrderCntType == 0)
    { 
        sps->log2MaxPicOrderCntLsbMinus4     = se(data, &bitsRead, &bytesRead, dataLength);
    } 
    else if (sps->picOrderCntType == 1)
    { 
        sps->deltaPicOrderAlwaysZeroFlag    = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
        sps->offsetForNonRefPic             = se(data, &bitsRead, &bytesRead, dataLength);
        sps->offsetForTopToBottomField      = se(data, &bitsRead, &bytesRead, dataLength);
        sps->numRefFramesInPicOrderCntCycle = ue(data, &bitsRead, &bytesRead, dataLength);

        sps->offsetForRefFrame              = (i32*) malloc(sizeof(i32) * sps->numRefFramesInPicOrderCntCycle);
        for (u32 i = 0; i < sps->numRefFramesInPicOrderCntCycle; i++) { 
            sps->offsetForRefFrame[i]       = se(data, &bitsRead, &bytesRead, dataLength);
        }
    }

    sps->numRefFrames                           = ue(data, &bitsRead, &bytesRead, dataLength); 
    sps->requiredFrameNumUpdateBehaviourFlag    = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    sps->picWidthInMbsMinus1                    = ue(data, &bitsRead, &bytesRead, dataLength); 
    sps->picHeightInMapUnitsMinus1              = ue(data, &bitsRead, &bytesRead, dataLength); 
    sps->frameMbsOnlyFlag                       = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);

    if (!(sps->frameMbsOnlyFlag)) { 
        sps->mbAdaptiveFrameFieldFlag           = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    }

    sps->direct8x8InferenceFlag                 = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    sps->vuiParametersPresentFlag               = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);

    if (sps->vuiParametersPresentFlag) { 
        //vui_parameters();
        printf("vui parameters present\n");
    }

    rbspTrailingBits(data, &bitsRead, &bytesRead, dataLength);

    printf("BITS READ %d\n", bitsRead);
    printf("TOTAL BITS %d\n", (dataLength * 8));
    printBits(data, 30);
    printf("===============\n");

    
    return sps;

    // TEMP FREEING
    if (sps->offsetForRefFrame != NULL)
        free(sps->offsetForRefFrame);

    free(sps);
}



void sliceHeaderDecode(u32 NALUnitDataLength, u8 *NALDataStream, sampleInfo *sample, MPEG_Data *videoData) { 
    NALUnitInfo *NALUnit = parseNALUnit(NALUnitDataLength, NALDataStream);

    u32 bitsRead = 0;
    u32 bytesRead = 0;

    u8 *data = NALUnit->NALUnitData;
    u32 dataLength = NALUnit->NALUnitDataLength;

    printf("===============\n");
    sliceHeader *sh = calloc(1, sizeof(sliceHeader));

    sh->firstMbInSlice      = ue(data, &bitsRead, &bytesRead, dataLength);
    sh->sliceType           = ue(data, &bitsRead, &bytesRead, dataLength);
    printf("slice type: %d\n", sh->sliceType);
    sh->picParameterSetId   = ue(data, &bitsRead, &bytesRead, dataLength);

    sh->frameNum; // get pic parameter set from last decode order slice then get sequence set id from there and use log2_max_frame_num_minus4 + 4 from there
    u32 frameMbsOnlyFlag; // get from sequence set

    u32 fieldPicFlag;
    u32 bottomFieldFlag;
    if (!frameMbsOnlyFlag) { 
        fieldPicFlag    = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
        if (fieldPicFlag) { 
            bottomFieldFlag    = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
        }
    }

    u32 NALUnitType; // defined in parent
    u32 idrPicId;
    if (NALUnitType == 5) { 
        idrPicId    = getUnsignedNBits(data, &bitsRead, &bytesRead, 1);
    }


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
