
#ifndef ENDIAN_UTIL_H
#define ENDIAN_UTIL_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

// u8-u64 stack ints interface
u8 littleEndianU8(u8 val);
u16 littleEndianU16(u16 val);
u32 littleEndianU32(u32 val);
u64 littleEndianU64(u64 val);

// i8-i64 stack ints interface
i8 littleEndianI8(i8 val);
i16 littleEndianI16(i16 val);
i32 littleEndianI32(i32 val);
i64 littleEndianI64(i64 val);

#endif // ENDIAN_UTIL_H