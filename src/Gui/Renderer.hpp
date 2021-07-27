#pragma once

#include <SDL2/SDL.h>

#include "../Common/Common.hpp"
#include "BackBuffer.hpp"

namespace Gui {

class Renderer {
   public:
    Renderer() = default;
    ~Renderer();

    SDL_Renderer *GetSDLRenderer() { return m_Renderer; }

    void Init(SDL_Window *, i32, i32);
    void PutPixel(i32, i32, Color);
    void Draw();
    void Clear(Color);
    inline void PutTexture(SDL_Texture *texture, SDL_Rect *position) {
        assert(texture != nullptr);
        assert(position != nullptr);
        m_BackBuffer.PutTexture(texture, position);
    }
    inline void DeleteTexture(SDL_Texture *texture) {
        assert(texture != nullptr);
        m_BackBuffer.DeleteTexture(texture);
    };

   private:
    SDL_Renderer *m_Renderer = nullptr;
    BackBuffer m_BackBuffer;
};

}  // namespace Gui