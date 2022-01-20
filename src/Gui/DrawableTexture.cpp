#include "DrawableTexture.hpp"

#include <SDL_render.h>
#include <SDL_surface.h>

#include "Renderer.hpp"
#include "glm/ext/vector_int2.hpp"

namespace GUI {

#define LOCK_AND_GET_PIXELS()   \
    SDL_LockSurface(m_Surface); \
    u32* pixels = (u32*)m_Surface->pixels

#define UNLOCK() SDL_UnlockSurface(m_Surface);

#define DO_WITH_PIXELS(...) \
    LOCK_AND_GET_PIXELS();  \
    {__VA_ARGS__};          \
    UNLOCK();

#define TO_PIXEL_COORDS(x, y) y*((u32)m_Surface->pitch / sizeof(u32)) + x

void DrawableTexture::PutPixel(glm::ivec2 position, const Color& c) {
    auto coords = TO_PIXEL_COORDS((u32)position.x, (u32)position.y);
    if (u32(m_Surface->h * m_Surface->w) < coords) return;

    DO_WITH_PIXELS(pixels[coords] = c;);

    if (m_Texture != nullptr) SDL_DestroyTexture(m_Texture);
    m_Texture = nullptr;
}

void DrawableTexture::Clear(const Color& c) {
    SDL_FillRect(m_Surface, nullptr, c);
    if (m_Texture != nullptr) SDL_DestroyTexture(m_Texture);
    m_Texture = nullptr;
}

DrawableTexture::DrawableTexture(glm::ivec2 size) {
    m_Surface = SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, 32,
                                               Color::PixelFormat);
}

DrawableTexture::~DrawableTexture() {
    SDL_FreeSurface(m_Surface);
    if (m_Texture != nullptr) SDL_DestroyTexture(m_Texture);
    m_Texture = nullptr;
}

SDL_Texture* DrawableTexture::Finalize(Renderer& r) {
    if (m_Texture == nullptr) {
        m_Texture = r.Convert(m_Surface);
    }

    return m_Texture;
}

}  // namespace GUI