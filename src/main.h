
#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <time.h>

// Preprocessor constants
#define TRUE 1
#define FALSE 0

//#define PRINT_ENABLE

#ifdef PRINT_ENABLE
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

// renaming stdint types
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// math functions
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(x, y)           (((x) > (y)) ? (x) : (y))
#endif

// benchmarking macros
#define TIMER_INIT     \
    float startTime;   \
    float endTime;     \
    float timeElapsed; \

#define TIMER_START() { \
    startTime = (float)clock()/CLOCKS_PER_SEC; \
}

#define TIMER_END() { \
    endTime = (float)clock()/CLOCKS_PER_SEC; \
    timeElapsed = endTime - startTime; \
    PRINTF("Elapsed: %f seconds\n", timeElapsed); \
}

// debug macros
#ifndef HERE
    #define HERE {PRINTF("HERE %d\n", g_here); g_here++;}
#endif // HERE

// error handling
typedef enum STATUS_CODE {
    OPERATION_SUCCESS = 0, 
    OPERATION_FAILED  = 1,
    OPERATION_NULL    = 2,

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

    // Color Table Errors
    COLOR_TABLE_NULL        = 80,
    COLOR_TABLE_MISSING     = 81,
    COLOR_TABLE_OVERFLOW    = 82,
} STATUS_CODE;

#define CHECKSTATUS(status)                     if (status != OPERATION_SUCCESS) { return status; }
#define CHECKSTATUS_CLOSE_FILE(status, gif)     if (status != OPERATION_SUCCESS) { fclose(gif); return status; }
#define CHECK_FWRITE_STATUS(returned, expected) if (returned != expected)       { return FWRITE_FAILED; }
#define CHECK_NULL(status)                      if (status == NULL)             { return OPERATION_NULL; }

#define ARRAY_NULL_CHECK(arr)                   if (arr == NULL)                { return ARRAY_NULL; }
#define BIT_ARRAY_NULL_CHECK(arr)               if (arr == NULL)                { return BIT_ARRAY_NULL; }
#define HASH_MAP_NULL_CHECK(map)                if (map == NULL)                { return HASH_MAP_NULL; }
#define LINKED_LIST_NULL_CHECK(list)            if (list == NULL)               { return LINKED_LIST_NULL; }
#define QUEUE_NULL_CHECK(queue)                 if (queue == NULL)              { return QUEUE_NULL; }

#define CANVAS_NULL_CHECK(canvas)               if (canvas == NULL)             { return CANVAS_NULL; }
#define FRAME_NULL_CHECK(frame)                 if (frame == NULL)              { return FRAME_NULL; }
#define COLOR_TABLE_NULL_CHECK(clrTable)        if (clrTable == NULL)           { return COLOR_TABLE_NULL; }
// #define CODE_TABLE_NULL_CHECK(cdTable)

#endif //MAIN_H