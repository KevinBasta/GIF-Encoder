
#ifndef GIF_EXTENDED_INTERFACE_H
#define GIF_EXTENDED_INTERFACE_H

#include <stdint.h>
#include "main.h"

#include "GIFInterface.h"

STATUS_CODE canvasUpdateWidthAndHeight(GIFCanvas *canvas, u16 canvasWidth, u16 canvasHeight);
STATUS_CODE canvasUpdateWidth(GIFCanvas *canvas, u16 canvasWidth);
STATUS_CODE canvasUpdateHeight(GIFCanvas *canvas, u16 canvasHeight);

STATUS_CODE frameUpdateWidthAndHeight(GIFFrame *frame, u16 frameWidth, u16 frameHeight);
STATUS_CODE frameUpdateWidth(GIFFrame *frame, u16 frameWidth);
STATUS_CODE frameUpdateHeight(GIFFrame *frame, u16 frameHeight);
STATUS_CODE frameUpdateImageLeftPosition(GIFFrame *frame, u16 imageLeftPosition);
STATUS_CODE frameUpdateImageTopPosition(GIFFrame *frame, u16 imageTopPosition);
STATUS_CODE frameUpdateDisposalMethod(GIFFrame *frame, u8 disposalMethod);
STATUS_CODE frameUpdateDelayTime(GIFFrame *frame, u16 delayTime);
STATUS_CODE frameRemoveTransparentColorIndex(GIFFrame *frame);

#endif // GIF_EXTENDED_INTERFACE_H
