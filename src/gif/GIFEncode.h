
#ifndef GIF_ENCODE_H
#define GIF_ENCODE_H

#include <stdint.h>
#include "main.h"

#include "GIFColorTable.h"
#include "GIFInterface.h"

STATUS_CODE encodeGIF(GIFCanvas *gifData);

#endif // GIF_ENCODE_H