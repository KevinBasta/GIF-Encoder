
#ifndef _GIF_ENCODER_STUB_H
#define _GIF_ENCODER_STUB_H

/**
 * A stub to include when using the encoder as a linkable library
 */

#ifndef GIF_ENCODER_MAIN_H
#define GIF_ENCODER_MAIN_H

#include <stdint.h>

// renaming stdint types
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef enum STATUS_CODE {
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

} STATUS_CODE;

#endif // GIF_ENCODER_MAIN_H

#ifndef GIF_ENCODER_ARRAY_H
#define GIF_ENCODER_ARRAY_H

typedef struct gifEncoder_array {
    size_t size;
    size_t currentIndex;
    u32 *items;
} gifEncoder_array;

#endif // GIF_ENCODER_ARRAY_H

#ifndef GIF_ENCODER_BIT_ARRAY_H
#define GIF_ENCODER_BIT_ARRAY_H

typedef struct gifEncoder_bitarray {
    size_t size;
    size_t currentIndex;
    u8     currentBit;
    
    bool   intervalInsertFlag;
    size_t intervalInsertBoundry;
    u8     intervalInsertItem;
    long   markIndex;
    
    u8 *items;
} gifEncoder_bitarray;

#endif // GIF_ENCODER_BIT_ARRAY_H

#ifndef GIF_ENCODER_HASHMAP_H
#define GIF_ENCODER_HASHMAP_H

typedef struct gifEncoder_HashMapEntry {
    void *key;
    void *value;
} gifEncoder_HashMapEntry;

typedef struct gifEncoder_HashMap {
    size_t size;
    size_t currentCount;
    gifEncoder_HashMapEntry **entries;
} gifEncoder_HashMap;

#endif // GIF_ENCODER_HASHMAP_H

#ifndef GIF_ENCODER_LINKED_LIST_H
#define GIF_ENCODER_LINKED_LIST_H

typedef struct gifEncoder_node { 
    void *item;
    void *nextNode;
} gifEncoder_node;

typedef struct linkedlist {
    size_t size;
    gifEncoder_node *head;
    gifEncoder_node *tail;
    gifEncoder_node *currentIter;
} gifEncoder_linkedlist;

#endif // GIF_ENCODER_LINKED_LIST_H

#ifndef GIF_ENCODER_COLOR_TABLE_H
#define GIF_ENCODER_COLOR_TABLE_H

typedef struct gifEncoder_RGB {
    u8 red;
    u8 green;
    u8 blue;
} gifEncoder_RGB;

typedef struct gifEncoder_colorTable {
    u32 lastIndex;
    u32 currentIndex;
    gifEncoder_RGB *items;
} gifEncoder_colorTable;

#endif // GIF_ENCODER_COLOR_TABLE_H

#ifndef GIF_ENCODER_CODE_TABLE_H
#define GIF_ENCODER_CODE_TABLE_H

typedef struct gifEncoder_codeTable {
    u32 index;
    gifEncoder_HashMap *map;
} gifEncoder_codeTable;

#endif // GIF_ENCODER_CODE_TABLE_H

#ifndef GIF_INTERFACE_H
#define GIF_INTERFACE_H

typedef struct gifEncoder_GIFCanvas {
    //
    // LogicalScreenDescriptor Fields Start
    //

    // In pixels
    u16 canvasWidth; 
    u16 canvasHeight;
    
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
    u8 packedField_GlobalColorTableFlag:   1;
    u8 packedField_ColorResolution:        3;
    u8 packedField_SortFlag:               1;
    u8 packedField_SizeOfGlobalColorTable: 3;
    
    //
    // Which index in the global color table should 
    // be used for pixels on the virtual canvas that 
    // aren't overlayed by an image.
    //
    u8 backgroundColorIndex;
    
    // Ignored Field
    u8 pixelAspectRatio;

    //
    // LogicalScreenDescriptor Fields End
    //

    //
    // Global color table only used if
    // packedField_GlobalColorTableFlag == 1
    //
    gifEncoder_colorTable *globalColorTable;

    gifEncoder_linkedlist *frames;

    //
    // General canvas data
    //
    char *fileName;

} gifEncoder_GIFCanvas;

typedef struct gifEncoder_GIFFrame {
    //
    // ImageDescriptor Fields Start
    //

    // Where the image should begin on the canvas
    u16 imageLeftPosition;
    u16 imageTopPosition;

    // In pixels
    u16 imageWidth;
    u16 imageHeight;

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
    u8 packedField_LocalColorTableFlag:   1;
    u8 packedField_InterlaceFlag:         1;
    u8 packedField_SortFlag:              1;
    u8 packedField_Reserved:              2;
    u8 packedField_SizeOfLocalColorTable: 3;
    
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
    u8 packedField_GCE_Reserved:             3;
    u8 packedField_GCE_DisposalMethod:       3;
    u8 packedField_GCE_UserInputFlag:        1;
    u8 packedField_GCE_TransparentColorFlag: 1;

    // number of hundredths of a second to wait before moving on to the next scene
    u16 delayTime;
    u8 transparentColorIndex;

    //
    // Graphic Control Extention Fields End
    //


    //
    // Local color table only used if
    // packedField_LocalColorTableFlag == 1
    //
    gifEncoder_colorTable *localColorTable;

    //
    // Stream of integers between 0 and 255
    // each integer being equivalent to a pixel
    // using it's value as the color table index
    //
    gifEncoder_array *indexStream;
    
    // 
    // Optimization fields to avoid
    // re-encoding the same frames
    //
    bool persistentFrameEncodeData;
    gifEncoder_bitarray *imageData;

} GIFFrame;

#endif // GIF_INTERFACE_H

//
// Main Interface
//

// Encode the canvas record
STATUS_CODE gifEncoder_createGIF(gifEncoder_GIFCanvas *canvas, bool freeCanvasRecord, bool createGIFOnError);

// Create a canvas record
gifEncoder_GIFCanvas *gifEncoder_canvasCreate(u16 canvasWidth, u16 canvasHeight);

STATUS_CODE gifEncoder_canvasAddGlobalColorTable(gifEncoder_GIFCanvas *canvas, gifEncoder_colorTable *clrTable);
STATUS_CODE gifEncoder_canvasCreateGlobalColorTable(gifEncoder_GIFCanvas *canvas);
STATUS_CODE gifEncoder_canvasAddColorToColorTable(gifEncoder_GIFCanvas *canvas, u8 red, u8 green, u8 blue);

STATUS_CODE gifEncoder_canvasSetBackgroundColorIndex(gifEncoder_GIFCanvas *canvas, u8 globalColorTableIndex);
STATUS_CODE gifEncoder_canvasAddFrame(gifEncoder_GIFCanvas *canvas, gifEncoder_GIFFrame *frame);

STATUS_CODE gifEncoder_canvasSetFileName(gifEncoder_GIFCanvas *canvas, char *newName);
void gifEncoder_freeCanvas(gifEncoder_GIFCanvas *canvas);

// Create a frame record
gifEncoder_GIFFrame *gifEncoder_frameCreate(u16 frameWidth, u16 frameHeight, u16 imageLeftPosition, u16 imageTopPosition);

STATUS_CODE gifEncoder_frameAddLocalColorTable(gifEncoder_GIFFrame *frame, gifEncoder_colorTable *clrTable);
STATUS_CODE gifEncoder_frameCreateLocalColorTable(gifEncoder_GIFFrame *frame);
STATUS_CODE gifEncoder_frameAddColorToColorTable(gifEncoder_GIFFrame *frame, u8 red, u8 green, u8 blue);

STATUS_CODE gifEncoder_frameAddIndexStream(gifEncoder_GIFFrame *frame, gifEncoder_array *indexStream);
STATUS_CODE gifEncoder_frameAddIndexStreamFromArray(gifEncoder_GIFFrame *frame, u8 stackArr[], size_t size);
STATUS_CODE gifEncoder_frameCreateIndexStream(gifEncoder_GIFFrame *frame, size_t indexStreamSize);
STATUS_CODE gifEncoder_frameAppendToIndexStream(gifEncoder_GIFFrame *frame, u32 item);

STATUS_CODE gifEncoder_frameAddGraphicsControlInfo(gifEncoder_GIFFrame *frame, u8 disposalMethod, u16 delayTime);
STATUS_CODE gifEncoder_frameSetTransparanetColorIndexInColorTable(gifEncoder_GIFFrame *frame, u8 transparentColorIndex);
void gifEncoder_freeFrame(gifEncoder_GIFFrame *frame);


//
// Extended Interface
//
STATUS_CODE gifEncoder_canvasUpdateWidthAndHeight(gifEncoder_GIFCanvas *canvas, u16 canvasWidth, u16 canvasHeight);
STATUS_CODE gifEncoder_canvasUpdateWidth(gifEncoder_GIFCanvas *canvas, u16 canvasWidth);
STATUS_CODE gifEncoder_canvasUpdateHeight(gifEncoder_GIFCanvas *canvas, u16 canvasHeight);
STATUS_CODE gifEncoder_canvasPrependFrame(gifEncoder_GIFCanvas *canvas, gifEncoder_GIFFrame *frame);

STATUS_CODE gifEncoder_frameUpdateWidthAndHeight(gifEncoder_GIFFrame *frame, u16 frameWidth, u16 frameHeight);
STATUS_CODE gifEncoder_frameUpdateWidth(gifEncoder_GIFFrame *frame, u16 frameWidth);
STATUS_CODE gifEncoder_frameUpdateHeight(gifEncoder_GIFFrame *frame, u16 frameHeight);
STATUS_CODE gifEncoder_frameUpdateImageLeftPosition(gifEncoder_GIFFrame *frame, u16 imageLeftPosition);
STATUS_CODE gifEncoder_frameUpdateImageTopPosition(gifEncoder_GIFFrame *frame, u16 imageTopPosition);
STATUS_CODE gifEncoder_frameUpdateDisposalMethod(gifEncoder_GIFFrame *frame, u8 disposalMethod);
STATUS_CODE gifEncoder_frameUpdateDelayTime(gifEncoder_GIFFrame *frame, u16 delayTime);
STATUS_CODE gifEncoder_frameRemoveTransparentColorIndex(gifEncoder_GIFFrame *frame);


//
// Transformations
//
STATUS_CODE gifEncoder_expandCanvas(gifEncoder_GIFCanvas *canvas, u32 widthMuliplier, u32 heightMuliplier);
STATUS_CODE gifEncoder_appendToFrame(gifEncoder_GIFFrame *frame, u8 *arrayToAppend, u32 widthOfAppendingArray, u32 heightOfAppendingArray, u32 trailingRows, u8 valueToUseForOverflow);


//
// Pattern encoding functions
//
STATUS_CODE gifEncoder_createErrorGif(u32 errorCode);
STATUS_CODE gifEncoder_createTypingGIF(char *sentence, bool addCursor);


#endif // _GIF_ENCODER_STUB_H