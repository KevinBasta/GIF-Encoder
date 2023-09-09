
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
#include "printUtility.h"

/**
 * @brief Write common GIF header to file
 * @param gif - Gif file to write to
 * @return OPERATION_SUCCESS or OPERATION_FAILED
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
 * @param canvas   - Struct containing required fields
 * @return OPERATION_SUCCESS or OPERATION_FAILED
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
 * @return OPERATION_SUCCESS or OPERATION_FAILED
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

STATUS_CODE encodeGraphicsControlExtension(FILE *gif, GIFCanvas *canvas) {
    size_t status;
    u32 nmemb = 1;

    // temporariy
    u8 buffer[] = {0x21, 0xF9, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};
    fwrite(&buffer, sizeof(u8) * 8, nmemb, gif);

    return OPERATION_SUCCESS;
}

/**
 * @brief Write every color in the color table to file
 * @param gif        - Gif file to write to
 * @param colorTable - Table to write to file
 * @return OPERATION_SUCCESS or OPERATION_FAILED
 */
STATUS_CODE encodeColorTable(FILE *gif, colorTable *colorTable) {
    size_t status;
    u32 nmemb = 1;

    for (int i = 0; i < colorTable->size; i++) {
        RGB entry = colorTable->arr[i];
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
 * @return OPERATION_SUCCESS or OPERATION_FAILED
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
 * @return OPERATION_SUCCESS or OPERATION_FAILED
 */
STATUS_CODE encodeTrailer(FILE *gif) {
    size_t status;
    u32 nmemb = 1;

    u8 endOfFileValue = 0x3B;
    status = fwrite(&endOfFileValue, sizeof(endOfFileValue), nmemb, gif);
    CHECK_FWRITE_STATUS(status, nmemb);

    return OPERATION_SUCCESS;
}


/**
 * @brief 
 * @param canvas 
 * @return 
 */
STATUS_CODE encodeGIF(GIFCanvas *canvas) {
    size_t status;
    FILE *gif = fopen("test.gif","wb");
    

    status = encodeHeader(gif);
    CHECKSTATUS(status);

    status = encodeLogicalScreenDescriptor(gif, canvas);
    CHECKSTATUS(status);

    // may need to pass in a gif struct that contains color resolution
    // (from packed field of image or canvas descriptors) for number of entries
    status = encodeColorTable(gif, canvas->globalColorTable);
    CHECKSTATUS(status);

    // status = encodeGraphicsControlExtension(gif, canvas);
    // CHECKSTATUS(status);

    GIFFrame *frame = malloc(sizeof(GIFFrame*));
    status = linkedlistYield(canvas->frames, (void**) (&frame));
    CHECKSTATUS(status);

    while (frame != NULL) {
        status = encodeImageDescriptor(gif, frame);
        CHECKSTATUS(status);

        status = encodeImageData(gif, canvas->globalColorTable, frame->indexStream);
        CHECKSTATUS(status);

        status = linkedlistYield(canvas->frames, (void**) (&frame));
        CHECKSTATUS(status);
    }

    status = encodeTrailer(gif);
    CHECKSTATUS(status);

    fclose(gif);

    return OPERATION_SUCCESS;
}
