
#ifndef _GIF_ENCODER_STUB_H
#define _GIF_ENCODER_STUB_H

/**
 * A stub to include when using the encoder as a linkable library
 */

#ifndef GIF_ENCODER_MAIN_H
#define GIF_ENCODER_MAIN_H

#include <stdint.h>

typedef enum GIF_ENCODER_STATUS_CODE {
    OPERATION_SUCCESS   = 0, 
    OPERATION_FAILED    = 1,
    OPERATION_NULL      = 2,
    OPERATION_OVERFLOW  = 3,

    // Array Errors
    ARRAY_NULL              = 10,
    ARRAY_REALLOC_FAILED    = 11,
    ARRAY_UNDERFLOW         = 12,
    ARRAY_INDEX_UNDEFINED   = 13,
 
    // Bit Array Errors
    BIT_ARRAY_NULL                           = 20,
    BIT_ARRAY_BOOKMARK_FAILED                = 21,
    BIT_ARRAY_REALLOC_FAILED                 = 22,
    BIT_ARRAY_PACK_OCCUPIED_GREATER_THAN_MIN = 23,

    // Hash Map Errors
    HASH_MAP_NULL           = 30,
    HASH_MAP_KEY_VAL_NULL   = 31,
    HASH_MAP_FULL           = 32,
    HASH_MAP_INSERT_TIMEOUT = 33,
    HASH_MAP_SEARCH_FAILED  = 34,

    // Linked List Errors
    LINKED_LIST_NULL        = 40,
    LINKED_LIST_NODE_NULL   = 41,

    // Queue Errors
    QUEUE_NULL  = 50,
    QUEUE_FULL  = 51,
    QUEUE_UNDERFLOW = 52,

    // File Errors
    FWRITE_FAILED = 60,

    // Interface Errors
    CANVAS_NULL                                = 70,
    CANVAS_BACKGROUND_WITHOUT_GLOBAL_CLR_TABLE = 71,
    CANVAS_BACKGROUND_INDEX_OUT_OF_BOUNDS      = 72,
    FRAME_NULL                                 = 73,
    FRAMES_TO_WRITE_ZERO                       = 74,
    FRAME_LOCAL_COLOR_TABLE_MISSING            = 75,
    FRAME_INDEX_STREAM_MISSING                 = 76,
    FRAME_INDEX_STREAM_WRONG_SIZE              = 77,
    FRAME_INDEX_STREAM_PIXEL_VALUE_INVALID     = 78, 

    // Color Table Errors
    COLOR_TABLE_NULL        = 80,
    COLOR_TABLE_MISSING     = 81,
    COLOR_TABLE_OVERFLOW    = 82,
    CODE_TABLE_NULL         = 83,

    // LZW Algorithm Errors
    LZW_INDEX_BUFFER_CONCAT_NULL        = 90,
    LZW_INDEX_BUFFER_PLUS_K_CONCAT_NULL = 91,
    LZW_INDEX_BUFFER_LAST_CONCAT_NULL   = 92,

} GIF_ENCODER_STATUS_CODE;

#endif // GIF_ENCODER_MAIN_H



#ifndef GIF_ENCODER_ARRAY_H
#define GIF_ENCODER_ARRAY_H

typedef struct gif_array {
    size_t size;
    size_t currentIndex;
    uint32_t *items;
} gif_array;

gif_array   *gif_arrayInit(size_t size);
GIF_ENCODER_STATUS_CODE gif_arrayAppend(gif_array *arr, uint32_t item);
GIF_ENCODER_STATUS_CODE gif_arrayPop(gif_array *arr);

GIF_ENCODER_STATUS_CODE gif_arrayGetItemAtIndex(gif_array *arr, size_t index, uint32_t *itemReturn);
uint32_t    gif_arrayGetItemAtCurrentIndex(gif_array *arr);
uint32_t    gif_arrayGetIncrement(gif_array *arr);

GIF_ENCODER_STATUS_CODE gif_arrayReset(gif_array *arr);
char        *gif_arrayConcat(gif_array *arr, char entrySeparator);
gif_array   *gif_arrayInitFromStackArray(uint8_t stackArr[], size_t size);

void gif_arrayPrint(gif_array *arr);
void gif_freeArray (gif_array *arr);

#endif // GIF_ENCODER_ARRAY_H



#ifndef GIF_ENCODER_BIT_ARRAY_H
#define GIF_ENCODER_BIT_ARRAY_H

typedef struct gif_bitarray {
    size_t size;
    size_t currentIndex;
    uint8_t     currentBit;
    
    bool   intervalInsertFlag;
    size_t intervalInsertBoundry;
    uint8_t     intervalInsertItem;
    long   markIndex;
    
    uint8_t *items;
} gif_bitarray;

#endif // GIF_ENCODER_BIT_ARRAY_H



#ifndef GIF_ENCODER_HASHMAP_H
#define GIF_ENCODER_HASHMAP_H

typedef struct gif_HashMapEntry {
    void *key;
    void *value;
} gif_HashMapEntry;

typedef struct gif_HashMap {
    size_t size;
    size_t currentCount;
    gif_HashMapEntry **entries;
} gif_HashMap;

#endif // GIF_ENCODER_HASHMAP_H



#ifndef GIF_ENCODER_LINKED_LIST_H
#define GIF_ENCODER_LINKED_LIST_H

typedef struct gif_node { 
    void *item;
    void *nextNode;
} gif_node;

typedef struct gif_linkedlist {
    size_t size;
    gif_node *head;
    gif_node *tail;
    gif_node *currentIter;
} gif_linkedlist;

#endif // GIF_ENCODER_LINKED_LIST_H



#ifndef GIF_ENCODER_COLOR_TABLE_H
#define GIF_ENCODER_COLOR_TABLE_H

typedef struct gif_RGB {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} gif_RGB;

typedef struct gif_colorTable {
    uint32_t lastIndex;
    uint32_t currentIndex;
    gif_RGB *items;
} gif_colorTable;

gif_colorTable *gif_colortableInit();
GIF_ENCODER_STATUS_CODE     gif_colortableAppendRGB(gif_colorTable *table, uint8_t red, uint8_t green, uint8_t blue);
void            gif_freeColorTable(gif_colorTable *table);

uint16_t gif_getLastColorIndex(int32_t colorTableInputSize);
uint8_t  gif_getLWZMinCodeSize(int32_t colorTableSize);
uint16_t gif_getClearCodeValue(int32_t colorTableSize);
uint16_t gif_getEOICodeValue(int32_t colorTableSize);

#endif // GIF_ENCODER_COLOR_TABLE_H



#ifndef GIF_ENCODER_CODE_TABLE_H
#define GIF_ENCODER_CODE_TABLE_H

typedef struct gif_codeTable {
    uint32_t index;
    gif_HashMap *map;
} gif_codeTable;

#endif // GIF_ENCODER_CODE_TABLE_H



#ifndef GIF_INTERFACE_H
#define GIF_INTERFACE_H

typedef struct GIFCanvas {
    //
    // LogicalScreenDescriptor Fields Start
    //

    // In pixels
    uint16_t canvasWidth; 
    uint16_t canvasHeight;
    
    //
    // Packed field:
    // [7]    : global color table flag
    //        : [1] global color table will follow
    //        : [0] no global color table
    //
    // [4-6]  : color resolution
    //        : 2^(n+1) computes entries in global color table
    //
    // [3]    : sort flag
    //        : [1] global color table sorted in decreasing importance
    //        : [0] not sorted
    //
    // [0-2]  : (!unused) called the "size of global color table"
    //
    uint8_t packedField_GlobalColorTableFlag:   1;
    uint8_t packedField_ColorResolution:        3;
    uint8_t packedField_SortFlag:               1;
    uint8_t packedField_SizeOfGlobalColorTable: 3;
    
    //
    // Which index in the global color table should 
    // be used for pixels on the virtual canvas that 
    // aren't overlayed by an image.
    //
    uint8_t backgroundColorIndex;
    
    // Ignored Field
    uint8_t pixelAspectRatio;

    //
    // LogicalScreenDescriptor Fields End
    //

    //
    // Global color table only used if
    // packedField_GlobalColorTableFlag == 1
    //
    gif_colorTable *globalColorTable;

    gif_linkedlist *frames;

    //
    // General canvas data
    //
    char *fileName;

} GIFCanvas;

typedef struct GIFFrame {
    //
    // ImageDescriptor Fields Start
    //

    // Where the image should begin on the canvas
    uint16_t imageLeftPosition;
    uint16_t imageTopPosition;

    // In pixels
    uint16_t imageWidth;
    uint16_t imageHeight;

    //
    // Packed field:
    // [7]   : local color table flag 
    //       : [1] use local color table that follows
    //       : [0] no local color table
    //
    // [6]   : interlace flag
    //       : changes the way images are rendered onto the screen to reduce flicker
    //       : requires encoding in a specific order 
    //
    // [5]   : sort flag 
    //       : [1] local color table sorted in decreasing importance 
    //       : [0] not sorted
    //
    // [3-4] : Reserved
    //
    // [0-2] : size of local color table
    //       : 2^(n+1) computes entries in local color table
    //
    uint8_t packedField_LocalColorTableFlag:   1;
    uint8_t packedField_InterlaceFlag:         1;
    uint8_t packedField_SortFlag:              1;
    uint8_t packedField_Reserved:              2;
    uint8_t packedField_SizeOfLocalColorTable: 3;
    
    //
    // ImageDescriptor Fields End
    //


    //
    // Graphic Control Extention Fields Start
    //

    //
    // Packed Field:
    // [5-7] : reserved for future use
    //
    // [2-4] : disposal method
    //       : [1] leave the image in place and draw the next image on top of it
    //       : [2] canvas should be restored to the background color
    //       : [3] restore the canvas to it's previous state before the current image was drawn
    //
    // [1]   : user input flag
    //       : [1] wait for some input from viewer beofre moving to next scene
    //       : [0] don't wait for input
    //
    // [0]   : transparent color flag
    //       : [1] transparancy used
    //       : [0] transparancy not used
    //
    uint8_t packedField_GCE_Reserved:             3;
    uint8_t packedField_GCE_DisposalMethod:       3;
    uint8_t packedField_GCE_UserInputFlag:        1;
    uint8_t packedField_GCE_TransparentColorFlag: 1;

    // number of hundredths of a second to wait before moving on to the next scene
    uint16_t delayTime;
    uint8_t transparentColorIndex;

    //
    // Graphic Control Extention Fields End
    //


    //
    // Local color table only used if
    // packedField_LocalColorTableFlag == 1
    //
    gif_colorTable *localColorTable;

    //
    // Stream of integers between 0 and 255
    // each integer being equivalent to a pixel
    // using it's value as the color table index
    //
    gif_array *indexStream;
    
    // 
    // Optimization fields to avoid
    // re-encoding the same frames
    //
    bool persistentFrameEncodeData;
    gif_bitarray *imageData;

} GIFFrame;

//
// Main Interface
//

// Encode the canvas record
GIF_ENCODER_STATUS_CODE gif_createGIF(GIFCanvas *canvas, bool freeCanvasRecord, bool createGIFOnError);

// Create a canvas record
GIFCanvas *gif_canvasCreate(uint16_t canvasWidth, uint16_t canvasHeight);

GIF_ENCODER_STATUS_CODE gif_canvasAddGlobalColorTable(GIFCanvas *canvas, gif_colorTable *clrTable);
GIF_ENCODER_STATUS_CODE gif_canvasCreateGlobalColorTable(GIFCanvas *canvas);
GIF_ENCODER_STATUS_CODE gif_canvasAddColorToColorTable(GIFCanvas *canvas, uint8_t red, uint8_t green, uint8_t blue);

GIF_ENCODER_STATUS_CODE gif_canvasSetBackgroundColorIndex(GIFCanvas *canvas, uint8_t globalColorTableIndex);
GIF_ENCODER_STATUS_CODE gif_canvasAddFrame(GIFCanvas *canvas, GIFFrame *frame);

GIF_ENCODER_STATUS_CODE gif_canvasSetFileName(GIFCanvas *canvas, char *newName);
void gif_freeCanvas(GIFCanvas *canvas);

// Create a frame record
GIFFrame *gif_frameCreate(uint16_t frameWidth, uint16_t frameHeight, uint16_t imageLeftPosition, uint16_t imageTopPosition);

GIF_ENCODER_STATUS_CODE gif_frameAddLocalColorTable(GIFFrame *frame, gif_colorTable *clrTable);
GIF_ENCODER_STATUS_CODE gif_frameCreateLocalColorTable(GIFFrame *frame);
GIF_ENCODER_STATUS_CODE gif_frameAddColorToColorTable(GIFFrame *frame, uint8_t red, uint8_t green, uint8_t blue);

GIF_ENCODER_STATUS_CODE gif_frameAddIndexStream(GIFFrame *frame, gif_array *indexStream);
GIF_ENCODER_STATUS_CODE gif_frameAddIndexStreamFromArray(GIFFrame *frame, uint8_t stackArr[], size_t size);
GIF_ENCODER_STATUS_CODE gif_frameCreateIndexStream(GIFFrame *frame, size_t indexStreamSize);
GIF_ENCODER_STATUS_CODE gif_frameAppendToIndexStream(GIFFrame *frame, uint32_t item);

GIF_ENCODER_STATUS_CODE gif_frameAddGraphicsControlInfo(GIFFrame *frame, uint8_t disposalMethod, uint16_t delayTime);
GIF_ENCODER_STATUS_CODE gif_frameSetTransparanetColorIndexInColorTable(GIFFrame *frame, uint8_t transparentColorIndex);
void gif_freeFrame(GIFFrame *frame);

#endif // GIF_INTERFACE_H



//
// Extended Interface
//
#ifndef GIF_EXTENDED_INTERFACE_H
#define GIF_EXTENDED_INTERFACE_H

GIF_ENCODER_STATUS_CODE gif_canvasUpdateWidthAndHeight(GIFCanvas *canvas, uint16_t canvasWidth, uint16_t canvasHeight);
GIF_ENCODER_STATUS_CODE gif_canvasUpdateWidth(GIFCanvas *canvas, uint16_t canvasWidth);
GIF_ENCODER_STATUS_CODE gif_canvasUpdateHeight(GIFCanvas *canvas, uint16_t canvasHeight);
GIF_ENCODER_STATUS_CODE gif_canvasPrependFrame(GIFCanvas *canvas, GIFFrame *frame);

GIF_ENCODER_STATUS_CODE gif_frameUpdateWidthAndHeight(GIFFrame *frame, uint16_t frameWidth, uint16_t frameHeight);
GIF_ENCODER_STATUS_CODE gif_frameUpdateWidth(GIFFrame *frame, uint16_t frameWidth);
GIF_ENCODER_STATUS_CODE gif_frameUpdateHeight(GIFFrame *frame, uint16_t frameHeight);
GIF_ENCODER_STATUS_CODE gif_frameUpdateImageLeftPosition(GIFFrame *frame, uint16_t imageLeftPosition);
GIF_ENCODER_STATUS_CODE gif_frameUpdateImageTopPosition(GIFFrame *frame, uint16_t imageTopPosition);
GIF_ENCODER_STATUS_CODE gif_frameUpdateDisposalMethod(GIFFrame *frame, uint8_t disposalMethod);
GIF_ENCODER_STATUS_CODE gif_frameUpdateDelayTime(GIFFrame *frame, uint16_t delayTime);
GIF_ENCODER_STATUS_CODE gif_frameRemoveTransparentColorIndex(GIFFrame *frame);

#endif // GIF_EXTENDED_INTERFACE_H



//
// Transformations
//
#ifndef GIF_TRANSFORMATIONS_H
#define GIF_TRANSFORMATIONS_H

GIF_ENCODER_STATUS_CODE gif_expandCanvas(GIFCanvas *canvas, uint32_t widthMuliplier, uint32_t heightMuliplier);
GIF_ENCODER_STATUS_CODE gif_appendToFrame(GIFFrame *frame, uint8_t *arrayToAppend, uint32_t widthOfAppendingArray, uint32_t heightOfAppendingArray, uint32_t trailingRows, uint8_t valueToUseForOverflow);

#endif // GIF_TRANSFORMATIONS_H



//
// Pattern encoding functions
//

#ifndef GIF_ERROR_H
#define GIF_ERROR_H

GIF_ENCODER_STATUS_CODE gif_createErrorGif(uint32_t errorCode);

#endif // GIF_ERROR_H


#ifndef GIF_TYPE_TEXT_H
#define GIF_TYPE_TEXT_H

GIF_ENCODER_STATUS_CODE gif_createTypingGIF(char *sentence, bool addCursor);

#endif // GIF_TYPE_TEXT_H


#endif // _GIF_ENCODER_STUB_H