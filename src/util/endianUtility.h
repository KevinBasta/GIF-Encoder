
#ifndef GIF_ENDIAN_UTIL_H
#define GIF_ENDIAN_UTIL_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

// u8-u64 stack ints interface
u8 gif_littleEndianU8(u8 val);
u16 gif_littleEndianU16(u16 val);
u32 gif_littleEndianU32(u32 val);
u64 gif_littleEndianU64(u64 val);

// i8-i64 stack ints interface
i8 gif_littleEndianI8(i8 val);
i16 gif_littleEndianI16(i16 val);
i32 gif_littleEndianI32(i32 val);
i64 gif_littleEndianI64(i64 val);

#endif // GIF_ENDIAN_UTIL_H