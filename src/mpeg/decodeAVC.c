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
    
    u32 bytesRead = 0;
    u32 sampleSize = sampleData->sampleSize;
    u32 unitLengthFieldSize = videoData->lengthSizeMinus1 + 1;
    // DEBUG printf("offset: %d\n", sampleData->sampleOffsetInMdat);
    
    for (int i = 0; i < sampleSize;) {
        // since NALUnitLength can be 1, 2, or 4 bytes, will just store it in 4 byte int
        u8 *NALUnitLength    = referenceNBytes(unitLengthFieldSize, sampleData->mdatPointer, &bytesRead);
        // DEBUG printHexNBytes(&(videoData->mdatBox->boxData[sampleData->sampleOffsetInMdat]), 4);
        
        // generalized is requied here, spesific functions won't work because the byteNumb would be assumed
        u32 NALUnitLengthInt = bigEndianCharToLittleEndianGeneralized(NALUnitLength, unitLengthFieldSize); 
        u8 *NALUnit          = referenceNBytes(NALUnitLengthInt, sampleData->mdatPointer, &bytesRead);
        //parseNALUnit(NALUnitLengthInt, NALUnit);
        i += unitLengthFieldSize + NALUnitLengthInt;

        // DEBUG printf("sample size: %7d nal unit length: %10u bytes read: %10u \n", sampleSize, NALUnitLengthInt, bytesRead);
    }
}

// very slow function
void parseNALUnit(u32 NALUnitLength, u8 *NALDataStream) { 
    NALUnitInfo *NALUnit = (NALUnitInfo*) malloc(sizeof(NALUnitInfo));

    u32 bytesRead = 0;
    u8 emulationPreventionByte = FALSE;

    u8 *zeroBitAndNALIdcAndUnitType = copyNBytes(1, NALDataStream, &bytesRead);
    u8 forbiddenZeroBit             = getNBits(1, 1, *zeroBitAndNALIdcAndUnitType);
    u8 NALRefIdc                    = getNBits(2, 3, *zeroBitAndNALIdcAndUnitType); 
    u8 NALUnitType                  = getNBits(4, 8, *zeroBitAndNALIdcAndUnitType);
    free(zeroBitAndNALIdcAndUnitType);
    //u32 test = bigEndianU8ArrToLittleEndianU32(zeroBitAndNALIdcAndUnitType);
    //printf("===========================%d\n", test);

    // DEBUG printf("nal ref id %d  nal unit type %d\n", NALRefIdc, NALUnitType);

    for (int i = 1; i < NALUnitLength; i++) { 
        if (i + 2 < NALUnitLength && bigEndianCharToLittleEndianGeneralized(checkNextNBytes(3, NALDataStream, bytesRead), 3) == 0x000003) { 
            emulationPreventionByte = TRUE;
            bytesRead += 2;
            i += 2;
            // DEBUG printBits(referenceNBytes(1, NALDataStream, &bytesRead), 1);
        } else { 
            bytesRead++;
        }
    }
    
    NALUnit->NALRefIdc = NALRefIdc;
    NALUnit->NALUnitType = NALUnitType;
    NALUnit->NALUnitData = &(NALDataStream[1]);
    
    if (emulationPreventionByte == TRUE) { 
        NALUnit->NALUnitLength = NALUnitLength - 1;
    } else { 
        NALUnit->NALUnitLength = NALUnitLength;
    }
    

    if (NALUnitType == 1) { 
        picParameterSetRbsp(NALUnit);
    }
}


void picParameterSetRbsp(NALUnitInfo *NALUnit) { 


}

