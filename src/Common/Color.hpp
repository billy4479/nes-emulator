#pragma once
#include "Common.hpp"
#include <SDL2/SDL_pixels.h>

struct Color {
  public:
    Color(u8 r, u8 g, u8 b, u8 a = 0xff) : r(r), g(g), b(b), a(a) {}
    operator u32() { return (b << 24) | (g << 16) | (r << 8) | a; }
    operator SDL_Color() { return SDL_Color{r, g, b, a}; }

    u8 r, g, b, a;

    static Color red;
    static Color blue;
    static Color green;
    static Color black;
    static Color white;
};