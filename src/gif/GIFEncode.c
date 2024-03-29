
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

/**
 * @brief Write common GIF header to file
 * @param gif - Gif file to write to
 * @return OPERATION_SUCCESS or error code
 */
static STATUS_CODE encodeHeader(FILE *gif) {
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
static STATUS_CODE encodeLogicalScreenDescriptor(FILE *gif, GIFCanvas *canvas) {
    size_t status;
    u32 nmemb = 1;

    u16 canvasWidth         = gif_littleEndianU16(canvas->canvasWidth);
    u16 canvasHeight        = gif_littleEndianU16(canvas->canvasHeight);
    
    u8 packedField          = packCanvasPackedField(canvas);
    gif_printBits(&packedField, sizeof(u8));

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
static STATUS_CODE encodeImageDescriptor(FILE *gif, GIFFrame *frame) {
    size_t status;
    u32 nmemb = 1;

    u8 imageSeparator = 0x2C;
    u16 imageLeftPosition   = gif_littleEndianU16(frame->imageLeftPosition);
    u16 imageTopPosition    = gif_littleEndianU16(frame->imageTopPosition);
    u16 imageWidth          = gif_littleEndianU16(frame->imageWidth);
    u16 imageHeight         = gif_littleEndianU16(frame->imageHeight);
    
    u8 packedField          = packFramePackedField(frame);
    gif_printIntBits(&packedField, sizeof(u8));

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
static STATUS_CODE encodeNetscapeLoopExtention(FILE *gif, u16 numberOfTimesToLoop) {
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
    numberOfTimesToLoop         = gif_littleEndianU16(numberOfTimesToLoop);
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

static STATUS_CODE encodeGraphicsControlExtension(FILE *gif, GIFFrame *frame) {
    size_t status;
    u32 nmemb = 1;

    u8 extensionIntroducer  = 0x21;
    u8 graphicsControlLabel = 0xF9;
    u8 byteSize             = 0x04;

    u8 packedField           = packGraphcisControlExtentionPackedField(frame);
    u16 delayTime            = gif_littleEndianU16(frame->delayTime);
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
static STATUS_CODE encodeColorTable(FILE *gif, gif_colorTable *colorTable) {
    size_t status;
    u32 nmemb = 1;

    for (int i = 0; i <= colorTable->lastIndex; i++) {
        gif_RGB entry = colorTable->items[i];
        
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
 * @param frame       - A frame containing index stream
 * @param clrTable    - Color table to use
 * @return OPERATION_SUCCESS or error code
 */
static STATUS_CODE encodeImageData(FILE *gif, GIFFrame *frame, gif_colorTable *clrTable) {
    size_t status;
    u32 nmemb = 1;

    gif_bitarray *imageData;
    if (frame->persistentFrameEncodeData && frame->imageData != NULL) {
        imageData = frame->imageData;
    } else {
        imageData = gif_bitarrayInit(frame->indexStream->size);
        status = gif_createLZWImageData(clrTable, frame->indexStream, imageData);
        CHECKSTATUS(status);

        if (frame->persistentFrameEncodeData) {
            frame->imageData = imageData;
        }
    }
    //PRINTF("\n");
    //gif_bitarrayPrint(imageData);

    for (u32 i = 0; i < imageData->currentIndex + 1; i++) {
        status = fwrite(&(imageData->items[i]), sizeof(u8), nmemb, gif);
        CHECK_FWRITE_STATUS(status, nmemb);
    }
    
    u8 lastByte = 0x00;    
    status = fwrite(&lastByte, sizeof(u8), nmemb, gif);

    if (!frame->persistentFrameEncodeData) {
        gif_freeBitArray(imageData);
    }

    return OPERATION_SUCCESS;
}

/**
 * @brief Write common GIF footer to file
 * @param gif - Gif file to write to
 * @return OPERATION_SUCCESS or error code
 */
static STATUS_CODE encodeTrailer(FILE *gif) {
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
 * @brief The following are validated:
 * [1]: The canvas is not null
 * [2]: The frames linked list is not null
 * [3]: The canvas linked list contains more than 0 frames
 * [4]: If no global color table, then each frame has a local color table
 * [5]: Each frame has an index stream
 * [6]: Each index stream size is equal to it's set length and width
 * [7]: Each index of the index stream is in the range of the color table
 * @param canvas    Record being validated
 * @return OPERATION_SUCCESS or error code
 */
static STATUS_CODE validateCanvasRecord(GIFCanvas *canvas) {
    STATUS_CODE status;

    // 1
    CANVAS_NULL_CHECK(canvas);

    // 2
    LINKED_LIST_NULL_CHECK(canvas->frames);

    // 3
    if (canvas->frames->size <= 0)
        return FRAMES_TO_WRITE_ZERO;

    bool hasGlobalColorTable = validGlobalColorTable(canvas);

    GIFFrame *frame;
    gif_linkedlistResetIter(canvas->frames);
    status = gif_linkedlistYield(canvas->frames, (void**) (&frame));
    CHECKSTATUS(status);

    while (frame != NULL) {
        bool hasLocalColorTable = validLocalColorTable(frame);
        
        // 4
        if (!hasGlobalColorTable && !hasLocalColorTable)
            return FRAME_LOCAL_COLOR_TABLE_MISSING;
        
        // 5
        if (frame->indexStream == NULL)
            return FRAME_INDEX_STREAM_MISSING;
        
        // 6
        if (frame->imageWidth * frame->imageHeight != frame->indexStream->size)
            return FRAME_INDEX_STREAM_WRONG_SIZE;

        // 7
        gif_colorTable *activeColorTable;
        if (hasLocalColorTable) {
            activeColorTable = frame->localColorTable;
        } else {
            activeColorTable = canvas->globalColorTable;
        }

        for (size_t i = 0; i < frame->indexStream->size; i++) {
            u32 pixelColorTableIndex = frame->indexStream->items[i];
            if (pixelColorTableIndex > activeColorTable->lastIndex) {
                return FRAME_INDEX_STREAM_PIXEL_VALUE_INVALID;
            }
        }

        status = gif_linkedlistYield(canvas->frames, (void**) (&frame));
        CHECKSTATUS(status);
    }

    gif_linkedlistResetIter(canvas->frames);

    return OPERATION_SUCCESS;
}

/**
 * @brief Set a flag in a frame for saving it's encoded image
 * data for resue only if the frame appears more than once
 * @param canvas    Canvas to 
 * @return 
 */
static STATUS_CODE markDuplicateFrames(GIFCanvas *canvas) {
    STATUS_CODE status;
    
    GIFFrame **uniqueFrames = calloc(canvas->frames->size, sizeof(GIFFrame*));
    size_t i = 0;

    GIFFrame *frame;
    gif_linkedlistResetIter(canvas->frames);
    status = gif_linkedlistYield(canvas->frames, (void**) (&frame));
    CHECKSTATUS(status);

    while (frame != NULL) {
        if (!gif_frameInArray(frame, uniqueFrames, i)) {
            uniqueFrames[i] = frame;
            i++;
        } else {
            frame->persistentFrameEncodeData = true;
        }
        
        status = gif_linkedlistYield(canvas->frames, (void**) (&frame));
        CHECKSTATUS(status);
    }

    gif_linkedlistResetIter(canvas->frames);

    free(uniqueFrames);

    return OPERATION_SUCCESS;
}

/**
 * @brief Encode a gif file given all the information required
 * @param canvas - Struct containing required information
 * @return OPERATION_SUCCESS or error code
 */
STATUS_CODE gif_encodeGIF(GIFCanvas *canvas) {
    size_t status;
   
    // Error checking data inputted
    status = validateCanvasRecord(canvas);
    CHECKSTATUS(status);

    // Data validation and optimizations
    status = markDuplicateFrames(canvas);
    CHECKSTATUS(status);

    FILE *gif;
    if (canvas->fileName != NULL) {
        gif = fopen(canvas->fileName,"wb");
    } else {
        gif = fopen("output.gif","wb");
    }

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
    gif_linkedlistResetIter(canvas->frames);
    status = gif_linkedlistYield(canvas->frames, (void**) (&frame));
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
            status = encodeImageData(gif, frame, frame->localColorTable);
        } else if (globalColorTableIsValid == true) {
            status = encodeImageData(gif, frame, canvas->globalColorTable);
        } else {
            fclose(gif);
            return COLOR_TABLE_MISSING;
        }
        CHECKSTATUS_CLOSE_FILE(status, gif);
        
        // Get the next frame in the linked list
        status = gif_linkedlistYield(canvas->frames, (void**) (&frame));
        CHECKSTATUS_CLOSE_FILE(status, gif);
    }

    // Encode the file terminator
    status = encodeTrailer(gif);
    CHECKSTATUS_CLOSE_FILE(status, gif);

    fclose(gif);

    return OPERATION_SUCCESS;
}
