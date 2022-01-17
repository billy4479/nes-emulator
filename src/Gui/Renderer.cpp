#include "Renderer.hpp"

#include <SDL_render.h>
#include <SDL_video.h>

namespace GUI {

Renderer::Renderer(std::string_view name, glm::ivec2 size)
    : m_EmulatorScreen(size) {
    assert(m_Renderer == nullptr);

    m_Window = SDL_CreateWindow(name.data(), SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, size.x, size.y, 0);

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
    assert(m_Renderer != nullptr);
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
}

void Renderer::Clear() { SDL_RenderClear(m_Renderer); }

SDL_Texture* Renderer::Convert(SDL_Surface* surface) {
    return SDL_CreateTextureFromSurface(m_Renderer, surface);
}

void Renderer::RenderToScreen() {
    DrawTexture(m_EmulatorScreen.Finalize(*this), {0, 0}, {1, 1});
    SDL_RenderPresent(m_Renderer);
}

static const glm::ivec2 CenterPointToCoords(const CenterPoint& center,
                                            SDL_Rect& dRect) {
    switch (center) {
        case CenterPoint::TOP_LEFT:
            return {0, 0};
            break;
        case CenterPoint::TOP_CENTER:
            return {dRect.w / 2, 0};
            break;
        case CenterPoint::TOP_RIGHT:
            return {dRect.w, 0};
            break;
        case CenterPoint::CENTER_LEFT:
            return {0, dRect.h / 2};
            break;
        case CenterPoint::CENTER_CENTER:
            return {dRect.w / 2, dRect.h / 2};
            break;
        case CenterPoint::CENTER_RIGHT:
            return {dRect.w, dRect.h / 2};
            break;
        case CenterPoint::BOTTOM_LEFT:
            return {0, dRect.h};
            break;
        case CenterPoint::BOTTOM_CENTER:
            return {dRect.w / 2, dRect.h};
            break;
        case CenterPoint::BOTTOM_RIGHT:
            return {dRect.w, dRect.h};
            break;
    }

    return {0, 0};
}

void Renderer::DrawTexture(SDL_Texture* texture, glm::ivec2 position,
                           glm::vec2 scale, f32 rotation, Color tint,
                           CenterPoint anchor, CenterPoint rotationCenter) {
    assert(texture != nullptr);

    i32 w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

    SDL_Rect sRect{0, 0, w, h};
    SDL_Rect dRect{position.x, position.y,
                   static_cast<i32>(abs(scale.x) * (f32)w),
                   static_cast<i32>(abs(scale.y) * (f32)h)};

    auto anchorPoint = CenterPointToCoords(anchor, dRect);
    auto rotationCenterPoint = CenterPointToCoords(rotationCenter, dRect);

    dRect.x -= anchorPoint.x;
    dRect.y -= anchorPoint.y;

    i32 flip = SDL_FLIP_NONE;
    if (scale.x < 0) flip |= SDL_FLIP_HORIZONTAL;
    if (scale.y < 0) flip |= SDL_FLIP_VERTICAL;

    SDL_Point rotationCenterPointSDL = {rotationCenterPoint.x,
                                        rotationCenterPoint.y};

    SDL_SetTextureColorMod(texture, tint.r, tint.g, tint.b);
    SDL_SetTextureAlphaMod(texture, tint.a);
    SDL_RenderCopyEx(m_Renderer, texture, &sRect, &dRect, rotation,
                     &rotationCenterPointSDL, (SDL_RendererFlip)flip);
}

GUI::DrawableTexture& Renderer::GetEmulatorScreen() { return m_EmulatorScreen; }
}  // namespace GUI