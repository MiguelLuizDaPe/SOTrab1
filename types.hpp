#pragma once

#include <cstddef>
#include <cfloat>
#include <cstdint>

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
static_assert(sizeof(f32) == 4, "`f32` must be 32 bit");

using f64 = double;
static_assert(sizeof(f64) == 8, "`f64` must be 64 bit");

using uint    = unsigned int;
using usize   = size_t;

using byte    = unsigned char;
using rune    = uint32_t;
using uintptr = uintptr_t;
