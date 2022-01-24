#pragma once

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

#ifdef _WIN32
    #define FUNC_SIG __FUNCSIG__
#else
    #define FUNC_SIG __PRETTY_FUNCTION__
#endif

#ifdef DEBUG
    #define ASSERT(check)                                                   \
        {                                                                   \
            if (!(check)) {                                                 \
                dbg_print("ASSERTion failed at %s:%d in function %s: %s\n", \
                          __FILE__, __LINE__, FUNC_SIG, #check);            \
                abort();                                                    \
            }                                                               \
        }

    #include <cstdio>
    #define dbg_print(...) fprintf(stdout, ##__VA_ARGS__)

    #define CHECK_SDL_ERROR()                                                 \
        {                                                                     \
            auto err = SDL_GetError();                                        \
            if (err != nullptr && strlen(err) != 0) {                         \
                dbg_print("SDL error occurred at %s:%d in function %s: %s\n", \
                          __FILE__, __LINE__, FUNC_SIG, err);                 \
                SDL_ClearError();                                             \
                abort();                                                      \
            }                                                                 \
        }

#else
    #define dbg_print(...)
    #define ASSERT(check) (void)(check)
    #define CHECK_SDL_ERROR()
#endif
