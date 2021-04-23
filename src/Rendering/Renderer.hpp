#pragma once

#include "../Types.hpp"
#include "BackBuffer.hpp"
#include <SDL2/SDL.h>
#include <stdexcept>

namespace Rendering {

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
} // namespace Rendering