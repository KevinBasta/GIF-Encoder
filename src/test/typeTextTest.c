
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

#include "array.h"
#include "GIFColorTable.h"
#include "GIFInterface.h"
#include "GIFExtendedInterface.h"
#include "GIFEncode.h"
#include "GIFTransformations.h"
#include "lettersAndNumbers.h"

#define ROW_HEIGHT_IN_PIXELS        11
#define LETTERS_PER_ROW             15

#define SPACES_AT_START_OF_LINE     1
#define SPACES_AT_END_OF_LINE       1
#define EMPTY_ROWS_BEFORE_TEXT      1
#define EMPTY_ROWS_AFTER_TEXT       1

// create a frame from a letter pattern
static GIFFrame *createLetterFrame(letterPattern *pattern, u16 imageLeftPosition, u16 imageTopPosition, u16 delayTime) {
    GIFFrame *frame = frameCreate(pattern->width, 
                                  pattern->height, 
                                  imageLeftPosition, 
                                  imageTopPosition + (ROW_HEIGHT_IN_PIXELS - pattern->height - pattern->baseline));
    frameAddIndexStreamFromArray(frame, pattern->pattern, pattern->width * pattern->height);
    frameAddGraphicsControlInfo(frame, 1, delayTime);

    return frame;
}

// Needs support for reusing frames when encoding for this to work efficiently. 
static void addBlinkingCursor(GIFCanvas *canvas, u16 imageLeftPosition, u16 imageTopPosition, u32 numberOfBlinks, u32 lengthOfBlink, u32 lengthOfClearBlink);

STATUS_CODE createTypingGIF(char *sentence, bool addCursor) {

    size_t numberOfLetters = strlen(sentence);

    u16 canvasWidth  = LETTERS_PER_ROW * MAX_LETTER_PIXEL_WIDTH;
    u16 canvasHeight = (u16) ceil((double) numberOfLetters / (double) LETTERS_PER_ROW) * ROW_HEIGHT_IN_PIXELS;
    u32 numberOfRows = EMPTY_ROWS_BEFORE_TEXT + 1 + EMPTY_ROWS_AFTER_TEXT;

    // Create canvas and set colors
    GIFCanvas *canvas = canvasCreate(canvasWidth, canvasHeight);
    canvasCreateGlobalColorTable(canvas);
    canvasAddColorToColorTable(canvas, 0, 0, 0);
    canvasAddColorToColorTable(canvas, 255, 255, 255);
    canvasSetBackgroundColorIndex(canvas, 0);

    // Must not use a stack string for strtok
    char *string = calloc(numberOfLetters + 1, sizeof(char));
    strncpy(string, sentence, numberOfLetters);
    string[numberOfLetters] = '\0';

    letterPattern *space = getLetterOrNumber(' ');

    u16 imageLeftPosition = (SPACES_AT_START_OF_LINE * space->width);
    u16 imageTopPosition  = (EMPTY_ROWS_BEFORE_TEXT * ROW_HEIGHT_IN_PIXELS);

    char *token = strtok(string, " ");
    while (token != NULL) {
        u32 wordLength          = strlen(token);
        u32 wordPixelWidth      = calculateWordPixelWidth(token, wordLength);
                
        // Start a new line if not enough space left in current line for this word
        if (imageLeftPosition + wordPixelWidth >= canvasWidth - (SPACES_AT_END_OF_LINE * space->width) && 
            wordLength <= LETTERS_PER_ROW) {
            imageTopPosition += ROW_HEIGHT_IN_PIXELS;
            imageLeftPosition = (SPACES_AT_START_OF_LINE * space->width);
            numberOfRows++;
        }

        // Add a frame for each letter of the word
        for (int i = 0; i <= wordLength; i++) {
            // Add a space at the start of each line
        
            // Pick the letter and get array pattern of the letter
            char letter;
            letterPattern *pattern;
            if (i == wordLength) {
                letter = ' ';
                pattern = space;
            } else {
                letter = token[i];
                pattern = getLetterOrNumber(letter);
            }

            // determine the speed the letter is typed
            u32 typingSpeed = 0;
            if (addCursor) {
                typingSpeed = 0;
            } else if (letter == ' ') {
                // Write spaces instantly
                typingSpeed = 1;
            } else if (letter == '.') {
                // Pause 1 second at dots
                typingSpeed = 100;
            } else {
                // make rand 5-15 or curve
                typingSpeed = 8;
            }

            // Create GIF frame for the letter and add to canvas
            GIFFrame *frame = createLetterFrame(pattern, imageLeftPosition, imageTopPosition, typingSpeed);
            canvasAddFrame(canvas, frame);

            // Move left and top positions to next letter start
            imageLeftPosition += pattern->width;
            if (imageLeftPosition + pattern->width >= canvasWidth - (SPACES_AT_END_OF_LINE * space->width)) {
                imageTopPosition += ROW_HEIGHT_IN_PIXELS;
                imageLeftPosition = (SPACES_AT_START_OF_LINE * space->width);
                numberOfRows++;
            }

            if (addCursor) {
                addBlinkingCursor(canvas, imageLeftPosition, imageTopPosition, 1, 20, 5);
            }
        }

        token = strtok(NULL, " ");
    }

    // Add a pause before looping
    if (addCursor) {
        addBlinkingCursor(canvas, imageLeftPosition, imageTopPosition, 4, 30, 10);
    } else {
        GIFFrame *frame = createLetterFrame(getLetterOrNumber('\0'), imageLeftPosition, imageTopPosition, 100);
        canvasAddFrame(canvas, frame);
    }

    canvasUpdateWidthAndHeight(canvas,
                               canvasWidth,
                               (numberOfRows * ROW_HEIGHT_IN_PIXELS));
    expandCanvas(canvas, 5, 5);

    createGIF(canvas, true, true);
    free(string);
}

/**
 * @brief !IMPORTANT! Just a proof of concept. cursor and clear cursor frames should be globals 
 * and need to ensure safe handling of freeing the frames. Also expanding frames
 * would need to keep track of the addresses of the frames already expanded.
 */
static void addBlinkingCursor(GIFCanvas *canvas, u16 imageLeftPosition, u16 imageTopPosition, u32 numberOfBlinks, u32 lengthOfBlink, u32 lengthOfClearBlink) {
    /* letterPattern *cursor       = getLetterOrNumber('_');


    letterPattern *clearCursor  = getLetterOrNumber('~');

    for (int i = 0; i < numberOfBlinks; i++) {
        GIFFrame *cursorFrame = frameCreate(cursor->width, 
                                        cursor->height, 
                                        imageLeftPosition, 
                                        imageTopPosition + (ROW_HEIGHT_IN_PIXELS - cursor->height - cursor->baseline));
        frameAddIndexStreamFromArray(cursorFrame, cursor->pattern, cursor->width * cursor->height);
        frameAddGraphicsControlInfo(cursorFrame, 1, lengthOfBlink);
        
        canvasAddFrame(canvas, cursorFrame);
        
        GIFFrame *clearCursorFrame = frameCreate(clearCursor->width, 
                                        clearCursor->height, 
                                        imageLeftPosition, 
                                        imageTopPosition + (ROW_HEIGHT_IN_PIXELS - clearCursor->height - clearCursor->baseline));
        frameAddIndexStreamFromArray(clearCursorFrame, clearCursor->pattern, clearCursor->width * clearCursor->height);
        frameAddGraphicsControlInfo(clearCursorFrame, 1, lengthOfClearBlink);

        canvasAddFrame(canvas, clearCursorFrame);
    } */
}

/* static void newLine(u16 *imageLeftPosition,
                    u16 *imageTopPosition, 
                    u32 *numberOfRows, 
                    letterPattern *space,
                    u32 wordLength,
                    u32 wordPixelWidth,
                    u16 canvasWidth) {
    if (*imageLeftPosition + wordPixelWidth > canvasWidth - (SPACES_AT_END_OF_LINE * space->width) && 
        wordLength <= LETTERS_PER_ROW) 
    {
        *imageLeftPosition = (SPACES_AT_START_OF_LINE * space->width);
        *imageTopPosition += ROW_HEIGHT_IN_PIXELS;
        (*numberOfRows)++;
    }
} */