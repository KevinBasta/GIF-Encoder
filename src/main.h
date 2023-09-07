
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
    OPERATION_SUCCESS = 1, 
    OPERATION_FAILED  = 0
} STATUS_CODE;

#define CHECKSTATUS(status)                     if (status == OPERATION_FAILED) { return OPERATION_FAILED; }
#define CHECK_FWRITE_STATUS(returned, expected) if (returned != expected)       { return OPERATION_FAILED; }
#define CHECK_NULL_RETURN(status)               if (status == NULL) { return OPERATION_FAILED; }

#endif //MAIN_H