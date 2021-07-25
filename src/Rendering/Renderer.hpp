#pragma once

#include "../Common.hpp"
#include "BackBuffer.hpp"
#include <SDL2/SDL.h>
#include <stdexcept>

namespace Rendering {
    class Renderer {
      public:
        Renderer() = default;
        ~Renderer();

        SDL_Renderer *GetSDLRenderer() { return m_Renderer; }

        void Init(SDL_Window *, i32, i32);
        void PutPixel(i32, i32, Color);
        void Draw();
        void Clear(Color);
        void PutTexture(SDL_Texture *, SDL_Rect *);
        void DeleteTexture(SDL_Texture *);

      private:
        SDL_Renderer *m_Renderer = nullptr;
        BackBuffer m_BackBuffer;
    };
} // namespace Rendering