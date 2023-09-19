
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#include <stdint.h>
#include "main.h"

#include "GIFInterface.h"
#include "GIFCodeTable.h"
#include "GIFColorTable.h"
#include "GIFLZWCompression.h"

#include "array.h"
#include "bitarray.h"
#include "bitUtility.h"
#include "endianUtility.h"
#include "searchUtility.h"
#include "printUtility.h"

// Avoid encoding frames twice
typedef struct duplicateFrameData {
    bitarray *imageData;
} duplicateFrameData;

/**
 * @brief Write common GIF header to file
 * @param gif - Gif file to write to
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE encodeHeader(FILE *gif) {
    size_t status;

    const char header[3]    = "GIF";
    const char version[3]   = "89a";
    u32 nmemb = 1;

    status = fwrite(header, sizeof(header), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);
    status = fwrite(version, sizeof(header), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    return OPERATION_SUCCESS;
}

static u8 packCanvasPackedField(GIFCanvas *canvas) {
    u8 packedField = 0;

    packedField |= canvas->packedField_GlobalColorTableFlag     << CANVAS_GLOBAL_COLOR_TABLE_FLAG_OFFSET;
    packedField |= canvas->packedField_ColorResolution          << CANVAS_COLOR_RESOLUTION_OFFSET;
    packedField |= canvas->packedField_SortFlag                 << CANVAS_SORT_FLAG_OFFSET;
    packedField |= canvas->packedField_SizeOfGlobalColorTable   << CANVAS_SIZE_OF_GLOBAL_COLOR_TABLE_OFFSET;

    return packedField;
}

/**
 * @brief Write general canvas information to file 
 * @param gif       - Gif file to write to
 * @param canvas    - Struct containing required fields
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE encodeLogicalScreenDescriptor(FILE *gif, GIFCanvas *canvas) {
    size_t status;
    u32 nmemb = 1;

    u16 canvasWidth         = littleEndianU16(canvas->canvasWidth);
    u16 canvasHeight        = littleEndianU16(canvas->canvasHeight);
    
    u8 packedField          = packCanvasPackedField(canvas);
    printBits(&packedField, sizeof(u8));

    // Represents which index in the global color table should be used 
    // for pixels on the virtual canvas that aren't overlayed by an image
    u8 backgroundColorIndex = canvas->backgroundColorIndex;
    u8 pixelAspectRatio     = canvas->pixelAspectRatio;

    status = fwrite(&canvasWidth, sizeof(canvasWidth), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&canvasHeight, sizeof(canvasHeight), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&packedField, sizeof(packedField), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&backgroundColorIndex, sizeof(backgroundColorIndex), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&pixelAspectRatio, sizeof(pixelAspectRatio), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    return OPERATION_SUCCESS;
}

static u8 packFramePackedField(GIFFrame *frame) {
    u8 packedField = 0;

    packedField |= frame->packedField_LocalColorTableFlag   << FRAME_LOCAL_COLOR_TABLE_FLAG_OFFSET;
    packedField |= frame->packedField_InterlaceFlag         << FRAME_INTERLACE_FLAG_OFFSET;
    packedField |= frame->packedField_SortFlag              << FRAME_SORT_FLAG_OFFSET;
    packedField |= frame->packedField_Reserved              << FRAME_RESERVED_OFFSET;
    packedField |= frame->packedField_SizeOfLocalColorTable << FRAME_SIZE_OF_LOCAL_COLOR_TABLE_OFFSET;

    return packedField;
}

/**
 * @brief write individual frame/image information to file
 * @param gif       - Gif file to write to
 * @param frame     - Struct containing required fields
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE encodeImageDescriptor(FILE *gif, GIFFrame *frame) {
    size_t status;
    u32 nmemb = 1;

    u8 imageSeparator = 0x2C;
    u16 imageLeftPosition   = littleEndianU16(frame->imageLeftPosition);
    u16 imageTopPosition    = littleEndianU16(frame->imageTopPosition);
    u16 imageWidth          = littleEndianU16(frame->imageWidth);
    u16 imageHeight         = littleEndianU16(frame->imageHeight);
    
    u8 packedField          = packFramePackedField(frame);
    printIntBits(&packedField, sizeof(u8));

    status = fwrite(&imageSeparator, sizeof(imageSeparator), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&imageLeftPosition, sizeof(imageLeftPosition), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&imageTopPosition, sizeof(imageTopPosition), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&imageWidth, sizeof(imageWidth), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&imageHeight, sizeof(imageHeight), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&packedField, sizeof(packedField), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    return OPERATION_SUCCESS;
}

/**
 * @brief Write extention to spesify number of loops
 * @param gif                   - Gif file to write to
 * @param numberOfTimesToLoop   - Zero (0) for infinite loop
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE encodeNetscapeLoopExtention(FILE *gif, u16 numberOfTimesToLoop) {
    size_t status;
    u32 nmemb = 1;

    u8 extensionIntroducer       = 0x21;
    u8 applicationExtensionLabel = 0xFF;

    // eleven bytes of data follow
    u8 lengthOfApplicationBlock = 0x0B; 
    char NETSCAPE[11]           = "NETSCAPE2.0";

    // three bytes of data follow
    u8 lengthOfDataSubblock     = 0x03;
    u8 subblockIntroducer       = 0x01;
    numberOfTimesToLoop         = littleEndianU16(numberOfTimesToLoop);
    u8 blockTerminator          = 0x00;

    status = fwrite(&extensionIntroducer, sizeof(extensionIntroducer), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&applicationExtensionLabel, sizeof(applicationExtensionLabel), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&lengthOfApplicationBlock, sizeof(lengthOfApplicationBlock), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(NETSCAPE, sizeof(NETSCAPE), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&lengthOfDataSubblock, sizeof(lengthOfDataSubblock), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&subblockIntroducer, sizeof(subblockIntroducer), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&numberOfTimesToLoop, sizeof(numberOfTimesToLoop), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&blockTerminator, sizeof(blockTerminator), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    return OPERATION_SUCCESS;
}

static u8 packGraphcisControlExtentionPackedField(GIFFrame *frame) {
    u8 packedField = 0;

    packedField |= frame->packedField_GCE_Reserved              << GRAPHICS_CONTROL_RESERVED_OFFSET;
    packedField |= frame->packedField_GCE_DisposalMethod        << GRAPHICS_CONTROL_DISPOSAL_METHOD_OFFSET;
    packedField |= frame->packedField_GCE_UserInputFlag         << GRAPHICS_CONTROL_USER_INPUT_FLAG_OFFSET;
    packedField |= frame->packedField_GCE_TransparentColorFlag  << GRAPHICS_CONTROL_TRANSPARENT_COLOR_FLAG_OFFSET;

    return packedField;
}

STATUS_CODE encodeGraphicsControlExtension(FILE *gif, GIFFrame *frame) {
    size_t status;
    u32 nmemb = 1;

    u8 extensionIntroducer  = 0x21;
    u8 graphicsControlLabel = 0xF9;
    u8 byteSize             = 0x04;

    u8 packedField           = packGraphcisControlExtentionPackedField(frame);
    u16 delayTime            = littleEndianU16(frame->delayTime);
    u8 transparentColorIndex = frame->transparentColorIndex;

    u8 blockTerminator      = 0x00;

    status = fwrite(&extensionIntroducer, sizeof(extensionIntroducer), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&graphicsControlLabel, sizeof(graphicsControlLabel), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&byteSize, sizeof(byteSize), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&packedField, sizeof(packedField), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&delayTime, sizeof(delayTime), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&transparentColorIndex, sizeof(transparentColorIndex), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    status = fwrite(&blockTerminator, sizeof(blockTerminator), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    return OPERATION_SUCCESS;
}

/**
 * @brief Write every color in the color table to file
 * @param gif        - Gif file to write to
 * @param colorTable - Table to write to file
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE encodeColorTable(FILE *gif, colorTable *colorTable) {
    size_t status;
    u32 nmemb = 1;

    for (int i = 0; i <= colorTable->lastIndex; i++) {
        RGB entry = colorTable->items[i];
        
        status = fwrite(&entry.red, sizeof(u8), nmemb, gif);
        CHECK_FWRITE_STATUS(status, nmemb);
        
        status = fwrite(&entry.green,sizeof(u8), nmemb, gif);
        CHECK_FWRITE_STATUS(status, nmemb);

        status = fwrite(&entry.blue, sizeof(u8), nmemb, gif);
        CHECK_FWRITE_STATUS(status, nmemb);
    }

    return OPERATION_SUCCESS;
}

/**
 * @brief Given a color table and index stream, generate the
 * corrosponding imageData using LZW and flexible code size compression
 * @param gif         - Gif file to write to
 * @param clrTable    - Color table to use
 * @param indexStream - A frame described in terms of the color table
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE encodeImageData(FILE *gif, colorTable *clrTable, array *indexStream) {
    size_t status;
    u32 nmemb = 1;

    bitarray *imageData = bitarrayInit(indexStream->size);
    status = createLZWImageData(clrTable, indexStream, imageData);
    CHECKSTATUS(status);
    //printf("\n");
    //bitarrayPrint(imageData);

    for (u32 i = 0; i < imageData->currentIndex; i++) {
        status = fwrite(&(imageData->items[i]), sizeof(u8), nmemb, gif);
        CHECK_FWRITE_STATUS(status, nmemb);
    }
    
    u8 lastByte = 0x00;    
    status = fwrite(&lastByte, sizeof(u8), nmemb, gif);

    freeBitArray(imageData);

    return OPERATION_SUCCESS;
}

/**
 * @brief Write common GIF footer to file
 * @param gif - Gif file to write to
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE encodeTrailer(FILE *gif) {
    size_t status;
    u32 nmemb = 1;

    u8 endOfFileValue = 0x3B;
    status = fwrite(&endOfFileValue, sizeof(endOfFileValue), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    return OPERATION_SUCCESS;
}


static bool validGlobalColorTable(GIFCanvas *canvas) {
    if (canvas->packedField_GlobalColorTableFlag == 1) {
        if (canvas->globalColorTable != NULL) {
            return true;
        }
    }

    return false;
}

static bool validLocalColorTable(GIFFrame *frame) {
    if (frame->packedField_LocalColorTableFlag == 1) {
        if (frame->localColorTable != NULL) {
            return true;
        }
    }

    return false;
}


/**
 * @brief Encode a gif file given all the information required
 * @param canvas - Struct containing required information
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE encodeGIF(GIFCanvas *canvas) {
    size_t status;
   
    // Error checking data inputted
    CANVAS_NULL_CHECK(canvas);
    LINKED_LIST_NULL_CHECK(canvas->frames);
    if (canvas->frames->size <= 0)
        return FRAMES_TO_WRITE_ZERO;
    
    FILE *gif = fopen("test.gif","wb");

    // Encode git header and canvas description
    status = encodeHeader(gif);
    CHECKSTATUS_CLOSE_FILE(status, gif);
    status = encodeLogicalScreenDescriptor(gif, canvas);
    CHECKSTATUS_CLOSE_FILE(status, gif);

    // Only encode global color table if flag is set
    bool globalColorTableIsValid = validGlobalColorTable(canvas);
    if (globalColorTableIsValid == true) {
        status = encodeColorTable(gif, canvas->globalColorTable);
        CHECKSTATUS_CLOSE_FILE(status, gif);
    }

    // Only add loop extention if there is more than one frame
    if (canvas->frames->size > 1) {
        status = encodeNetscapeLoopExtention(gif, 0);
        CHECKSTATUS_CLOSE_FILE(status, gif);
    }

    GIFFrame *frame;
    linkedlistResetIter(canvas->frames);
    status = linkedlistYield(canvas->frames, (void**) (&frame));
    CHECKSTATUS_CLOSE_FILE(status, gif);

    while (frame != NULL) {
        // Encode graphics control extention
        status = encodeGraphicsControlExtension(gif, frame);
        CHECKSTATUS_CLOSE_FILE(status, gif);

        // Encode image description
        status = encodeImageDescriptor(gif, frame);
        CHECKSTATUS_CLOSE_FILE(status, gif);

        // Only encode local color table if flag is set
        bool localColorTableIsValid = validLocalColorTable(frame);
        if (localColorTableIsValid == true) {
            status = encodeColorTable(gif, frame->localColorTable);
            CHECKSTATUS_CLOSE_FILE(status, gif);
        }

        // Encode the frame's LZW compressed indexStream
        if (localColorTableIsValid == true) {
            status = encodeImageData(gif, frame->localColorTable, frame->indexStream);
        } else if (globalColorTableIsValid == true) {
            status = encodeImageData(gif, canvas->globalColorTable, frame->indexStream);
        } else {
            fclose(gif);
            return COLOR_TABLE_MISSING;
        }
        CHECKSTATUS_CLOSE_FILE(status, gif);
        
        // Get the next frame in the linked list
        status = linkedlistYield(canvas->frames, (void**) (&frame));
        CHECKSTATUS_CLOSE_FILE(status, gif);
    }

    // Encode the file terminator
    status = encodeTrailer(gif);
    CHECKSTATUS_CLOSE_FILE(status, gif);

    fclose(gif);

    return OPERATION_SUCCESS;
}
