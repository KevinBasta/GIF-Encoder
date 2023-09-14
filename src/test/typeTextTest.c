
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFEncode.h"
#include "GIFTransformations.h"
#include "lettersAndNumber.h"

#define LETTERS_PER_ROW             10

STATUS_CODE createTypingGIF(char *sentence) {

    size_t numberOfLetters = strlen(sentence);

    u16 canvasWidth  = LETTERS_PER_ROW * LETTER_PIXEL_WIDTH;
    u16 canvasHeight = (u16) ceil((double) numberOfLetters / (double) LETTERS_PER_ROW) * LETTER_PIXEL_HEIGHT;
    u32 numberOfRows = 1;

    // Create canvas and set colors
    GIFCanvas *canvas = canvasCreate(canvasWidth, canvasHeight);
    canvasCreateGlobalColorTable(canvas);
    canvasAddColorToColorTable(canvas, 0, 0, 0);
    canvasAddColorToColorTable(canvas, 255, 255, 255);
    canvasSetBackgroundColorIndex(canvas, 0);

    u16 imageLeftPosition = 0;
    u16 imageTopPosition  = 0;

    // Must not use a stack string for strtok
    char *string = calloc(numberOfLetters + 1, sizeof(char));
    strncpy(string, sentence, numberOfLetters);
    string[numberOfLetters] = '\0';

    char *token = strtok(string, " ");
    while (token != NULL) {
        u32 wordLength = strlen(token);
        u32 wordWithSpaceLength = wordLength + 1;

        if (imageLeftPosition + (wordWithSpaceLength * LETTER_PIXEL_WIDTH) > canvasWidth && wordWithSpaceLength <= LETTERS_PER_ROW) {
            imageTopPosition += LETTER_PIXEL_HEIGHT;
            imageLeftPosition = 0;
            numberOfRows++;
        }

        for (int i = 0; i <= wordWithSpaceLength; i++) {
            char letter = token[i];
            if (letter == '\0')
                letter = ' ';

            u8 *pattern = getLetterOrNumber(letter);

            GIFFrame *frame = frameCreate(LETTER_PIXEL_WIDTH, LETTER_PIXEL_HEIGHT, imageLeftPosition, imageTopPosition);
            frameCreateIndexStreamFromArray(frame, pattern, LETTER_PIXEL_WIDTH * LETTER_PIXEL_HEIGHT);
            if (letter == ' ') {
                // Write spaces instantly
                frameAddGraphicsControlInfo(frame, 1, 0);
            } else {
                frameAddGraphicsControlInfo(frame, 1, 20); // make rand 15-25
            }
            
            canvasAddFrame(canvas, frame);
            imageLeftPosition += LETTER_PIXEL_WIDTH;
        }

        token = strtok(NULL, " ");
    }

    canvasUpdateWidthAndHeight(canvas, canvasWidth, numberOfRows * LETTER_PIXEL_HEIGHT);
    expandCanvas(canvas, 20, 20);

    createGIFAndFreeCanvas(canvas);
    free(string);
}