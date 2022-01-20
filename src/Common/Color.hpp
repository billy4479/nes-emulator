#pragma once
#include <SDL2/SDL_pixels.h>

#include "Common.hpp"

struct Color {
   public:
    Color() : Color(0, 0, 0, 0xff) {}
    Color(u8 r, u8 g, u8 b, u8 a = 0xff) : r(r), g(g), b(b), a(a) {}
    inline operator u32() const {
        u32 res = ((u32)r << 24) | ((u32)g << 16) | ((u32)b << 8) | a;
        return res;
    }
    inline operator SDL_Color() const { return SDL_Color{r, g, b, a}; }

    u8 r, g, b, a;

    static constexpr u32 PixelFormat = SDL_PIXELFORMAT_RGBA8888;

    static Color red;
    static Color blue;
    static Color green;
    static Color black;
    static Color white;

    static Color hsl(u16 h, f32 s, f32 l, u8 a = 0xff);
};