#pragma once

#include "BackBuffer.hpp"
#include "Types.hpp"
#include <SDL2/SDL.h>
#include <stdexcept>

class Renderer {
  public:
    Renderer() = default;
    ~Renderer();

    operator SDL_Renderer *() { return m_Renderer; }

    void Init(SDL_Window *window, i32 w, i32 h);
    void PutPixel(i32 x, i32 y, Color c);
    void Draw();
    void Clear(Color c);

  private:
    SDL_Renderer *m_Renderer = nullptr;
    BackBuffer m_BackBuffer;
};