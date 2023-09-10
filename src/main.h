
#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <time.h>

// Preprocessor constants
#define TRUE 1
#define FALSE 0
#define DEBUG_PRINT_ENABLE TRUE
#define DEBUG_PRINT(...)  print(__VA_ARGS__)
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#define PRINT_INT(X) printf("%d\n", X) 

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
    printf("Elapsed: %f seconds\n", timeElapsed); \
}

// error handling
typedef enum STATUS_CODE {
    OPERATION_SUCCESS = 0, 
    OPERATION_FAILED  = 1,
    OPERATION_NULL    = 2,

    // Hashmap Errors


    // Array Errors
    ARRAY_NULL      = 20,

    // Linked List Errors
    LINKED_LIST_NULL = 31,

    // Bit Array Errors


    // File Errors
    FWRITE_FAILED = 40,

    // Interface Errors
    CANVAS_NULL             = 50,
    FRAME_NULL              = 51,
    FRAMES_TO_WRITE_ZERO    = 52,

    // Color Table Errors
    COLOR_TABLE_NULL = 60,
    COLOR_TABLE_MISSING = 61,

} STATUS_CODE;

#define CHECKSTATUS(status)                     if (status != OPERATION_SUCCESS) { return status; }
#define CHECKSTATUS_CLOSE_FILE(status, gif)     if (status != OPERATION_SUCCESS) { fclose(gif); return status; }
#define CHECK_FWRITE_STATUS(returned, expected) if (returned != expected)       { return FWRITE_FAILED; }
#define CHECK_NULL(status)                      if (status == NULL)             { return OPERATION_NULL; }

#define ARRAY_NULL_CHECK(arr)                   if (arr == NULL)                { return ARRAY_NULL; }
#define LINKED_LIST_NULL_CHECK(list)            if (list == NULL)               { return LINKED_LIST_NULL; }

#define CANVAS_NULL_CHECK(canvas)               if (canvas == NULL)             { return CANVAS_NULL; }
#define FRAME_NULL_CHECK(frame)                 if (frame == NULL)              { return FRAME_NULL; }
#define COLOR_TABLE_NULL_CHECK(clrTable)        if (clrTable == NULL)           { return COLOR_TABLE_NULL; }


#endif //MAIN_H