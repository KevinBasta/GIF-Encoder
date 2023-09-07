#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "main.h"

#include "searchUtility.h"

#include "hashmap.h"
#include "array.h"
#include "GIFInterface.h"

#include "bitUtility.h"
#include "endianUtility.h"
#include "printUtility.h"
#include "memoryManagement.h"

#include "GIFEncode.h"
#include "GIFCodeTable.h"
#include "GIFError.h"

STATUS_CODE gifErrorOutput() {
    // creates a gif that displays
    // the word "ERROR!" in case the main
    // program goes wrong somewhere
    // also serves as a good test


    return OPERATION_SUCCESS;
}


int main(int argc, char **argv) { 

    TIMER_INIT;
    TIMER_START();
    
    createErrorGif();
    
    TIMER_END();
    
    printf("end of script\n");
    exit(0);
}