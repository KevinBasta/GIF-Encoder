
#ifndef GIF_EXTENDED_INTERFACE_H
#define GIF_EXTENDED_INTERFACE_H

#include <stdint.h>
#include "main.h"

#include "GIFInterface.h"

STATUS_CODE gif_canvasUpdateWidthAndHeight(GIFCanvas *canvas, u16 canvasWidth, u16 canvasHeight);
STATUS_CODE gif_canvasUpdateWidth(GIFCanvas *canvas, u16 canvasWidth);
STATUS_CODE gif_canvasUpdateHeight(GIFCanvas *canvas, u16 canvasHeight);
STATUS_CODE gif_canvasPrependFrame(GIFCanvas *canvas, GIFFrame *frame);

STATUS_CODE gif_frameUpdateWidthAndHeight(GIFFrame *frame, u16 frameWidth, u16 frameHeight);
STATUS_CODE gif_frameUpdateWidth(GIFFrame *frame, u16 frameWidth);
STATUS_CODE gif_frameUpdateHeight(GIFFrame *frame, u16 frameHeight);
STATUS_CODE gif_frameUpdateImageLeftPosition(GIFFrame *frame, u16 imageLeftPosition);
STATUS_CODE gif_frameUpdateImageTopPosition(GIFFrame *frame, u16 imageTopPosition);
STATUS_CODE gif_frameUpdateDisposalMethod(GIFFrame *frame, u8 disposalMethod);
STATUS_CODE gif_frameUpdateDelayTime(GIFFrame *frame, u16 delayTime);
STATUS_CODE gif_frameRemoveTransparentColorIndex(GIFFrame *frame);

#endif // GIF_EXTENDED_INTERFACE_H
