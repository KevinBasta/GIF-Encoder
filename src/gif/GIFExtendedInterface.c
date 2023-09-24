
#include <stdint.h>
#include "main.h"

#include "GIFInterface.h"

// Canvas value updaters
WASM_EXPORT STATUS_CODE canvasUpdateWidthAndHeight(GIFCanvas *canvas, u16 canvasWidth, u16 canvasHeight) {
    CANVAS_NULL_CHECK(canvas);

    canvas->canvasWidth  = canvasWidth;
    canvas->canvasHeight = canvasHeight;

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE canvasUpdateWidth(GIFCanvas *canvas, u16 canvasWidth) {
    CANVAS_NULL_CHECK(canvas);

    canvas->canvasWidth = canvasWidth;

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE canvasUpdateHeight(GIFCanvas *canvas, u16 canvasHeight) {
    CANVAS_NULL_CHECK(canvas);

    canvas->canvasHeight = canvasHeight;

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE canvasPrependFrame(GIFCanvas *canvas, GIFFrame *frame) {
    STATUS_CODE status;
    
    CANVAS_NULL_CHECK(canvas);
    FRAME_NULL_CHECK(frame);

    if (canvas->frames == NULL) {
        canvas->frames = linkedlistInit();
    }

    status = linkedlistPrepend(canvas->frames, frame);
    CHECKSTATUS(status);

    return OPERATION_SUCCESS;
}


// Frame value updaters
WASM_EXPORT STATUS_CODE frameUpdateWidthAndHeight(GIFFrame *frame, u16 frameWidth, u16 frameHeight) {
    FRAME_NULL_CHECK(frame);

    frame->imageWidth  = frameWidth;
    frame->imageHeight = frameHeight;

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE frameUpdateWidth(GIFFrame *frame, u16 frameWidth) {
    FRAME_NULL_CHECK(frame);

    frame->imageWidth = frameWidth;

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE frameUpdateHeight(GIFFrame *frame, u16 frameHeight) {
    FRAME_NULL_CHECK(frame);

    frame->imageHeight = frameHeight;

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE frameUpdateImageLeftPosition(GIFFrame *frame, u16 imageLeftPosition) {
    FRAME_NULL_CHECK(frame);

    frame->imageLeftPosition = imageLeftPosition;

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE frameUpdateImageTopPosition(GIFFrame *frame, u16 imageTopPosition) {
    FRAME_NULL_CHECK(frame);

    frame->imageTopPosition = imageTopPosition;

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE frameUpdateDisposalMethod(GIFFrame *frame, u8 disposalMethod) {
    FRAME_NULL_CHECK(frame);

    frame->packedField_GCE_DisposalMethod = disposalMethod;

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE frameUpdateDelayTime(GIFFrame *frame, u16 delayTime) {
    FRAME_NULL_CHECK(frame);

    frame->delayTime = delayTime;

    return OPERATION_SUCCESS;
}

WASM_EXPORT STATUS_CODE frameRemoveTransparentColorIndex(GIFFrame *frame) {
    FRAME_NULL_CHECK(frame);

    frame->packedField_GCE_TransparentColorFlag = 0;
    frame->transparentColorIndex                = 0;

    return OPERATION_SUCCESS;
}
