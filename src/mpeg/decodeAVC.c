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
    printf("bytes read: %d\n", bytesRead);
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

    if (NALUnitType == 1) { 
        picParameterSetRbsp(NALUnit);
    }
}


void picParameterSetRbsp(NALUnitInfo *NALUnit) { 
    u32 leadingZeroBits;

    u32 bitsRead = 0;
    u32 bytesRead = 0;

    u8 *data = NALUnit->NALUnitData;
    u32 dataLength = NALUnit->NALUnitDataLength;
    printBits(data, 10);

    printf("===============\n");

    u32 picParameterSetId  = getCodeNum(data, &bitsRead, &bytesRead, dataLength);
    printf("entry 1: %d\n\n", picParameterSetId);

    u32 seqParameterSetId  = getCodeNum(data, &bitsRead, &bytesRead, dataLength);
    printf("entry 2: %d\n\n", seqParameterSetId);

    u32 entropyCodingModeFlag  = getCodeNum(data, &bitsRead, &bytesRead, dataLength);
    printf("entry 2: %d\n\n", entropyCodingModeFlag);

    u32 picOrderPresentFlag;
    
    printf("===============\n");

    //u32 picParameterSetId   = countBitsToFirstNonZero(NALUnit->NALUnitDataLength, &bitsRead, &bytesRead, NALUnit->NALUnitDataLength);

}

u32 getCodeNum(u8 *data, u32 *bitsRead, u32 *bytesRead, u32 dataLength) { 
    u32 leadingZeroBits             = countBitsToFirstNonZero(data, bitsRead, bytesRead, dataLength);
    //printf("bits bytes 1: %d %d\n", *bitsRead, *bytesRead);

    u32 leadingBitsOffsetPlusTwo    = *bitsRead;
    u8 *bigEndianDataReference      = referenceNBits(leadingZeroBits, data, bitsRead, bytesRead);
    u32 postReferenceBitsOffset     = *bitsRead;
    //printf("bits bytes 2: %d %d\n", *bitsRead, *bytesRead);

    u32 readBits                    = simpleBigEndianToLittleEndianBits(bigEndianDataReference, leadingBitsOffsetPlusTwo, postReferenceBitsOffset, leadingZeroBits);
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
