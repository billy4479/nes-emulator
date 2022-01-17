#pragma once

#include <SDL.h>

#include <glm/ext/vector_int2.hpp>

#include "../Common/Color.hpp"
#include "../Common/Common.hpp"

namespace GUI {
class Renderer;

class DrawableTexture {
   public:
    void PutPixel(glm::ivec2 position, const Color& c);
    void Clear(const Color& c);

   public:
    DrawableTexture(glm::ivec2 size);
    ~DrawableTexture();

   private:
    SDL_Texture* Finalize(Renderer&);

    SDL_Surface* m_Surface = nullptr;
    SDL_Texture* m_Texture = nullptr;
    i32 m_Width, m_Height;

    friend class Renderer;
};
}  // namespace GUI