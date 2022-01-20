#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

#ifndef NDEBUG
    #define DEBUG 1
#endif

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;

using std::size_t;

#ifdef DEBUG
    #include <cstdio>
    #define dbg_print(...) fprintf(stdout, ##__VA_ARGS__)
#else
    #define dbg_print(...) 0
#endif
