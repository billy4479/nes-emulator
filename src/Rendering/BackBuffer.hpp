#pragma once
#include "../Color.hpp"
#include "../Types.hpp"
#include <SDL2/SDL.h>
#include <assert.h>

namespace Rendering {
class BackBuffer {
  public:
    BackBuffer() = default;
    ~BackBuffer();

    void Init(i32 width, i32 height, SDL_Renderer *renderer);
    void Clear(Color c);
    void Swap();
    u32 &At(i32 x, i32 y);

  private:
    SDL_Texture *m_Texture = nullptr;
    SDL_Renderer *m_Renderer = nullptr;
    u32 *m_BackBuffer = nullptr;
    i32 m_Width, m_Height, m_Pitch;
};
} // namespace Rendering
