#include "Renderer.hpp"

#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_video.h>

namespace GUI {

Renderer::Renderer(std::string_view name, glm::ivec2 size) {
    ASSERT(m_Renderer == nullptr);

    ASSERT(SDL_Init(SDL_INIT_EVERYTHING) == 0);
    CHECK_SDL_ERROR();
    ASSERT(TTF_Init() == 0);
    CHECK_SDL_ERROR();

    m_Window = SDL_CreateWindow(name.data(), SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, size.x, size.y, 0);
    CHECK_SDL_ERROR();

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
    CHECK_SDL_ERROR();
    ASSERT(m_Renderer != nullptr);
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
    SDL_RenderPresent(m_Renderer);
    CHECK_SDL_ERROR();
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

void Renderer::DrawTextureInternal(SDL_Texture* texture, glm::ivec2 position,
                                   glm::vec2 scale, f32 rotation, Color tint,
                                   CenterPoint anchor,
                                   CenterPoint rotationCenter,
                                   const SDL_Rect* sourceRect) {
    ASSERT(texture != nullptr);

    i32 w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    CHECK_SDL_ERROR();

    SDL_Rect sRect;
    if (sourceRect == nullptr)
        sRect = {0, 0, w, h};
    else
        sRect = *sourceRect;

    SDL_Rect dRect = {position.x, position.y,
                      static_cast<i32>(abs(scale.x) * (f32)sRect.w),
                      static_cast<i32>(abs(scale.y) * (f32)sRect.h)};

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
    CHECK_SDL_ERROR();

    SDL_SetTextureAlphaMod(texture, tint.a);
    CHECK_SDL_ERROR();

    SDL_RenderCopyEx(m_Renderer, texture, &sRect, &dRect, rotation,
                     &rotationCenterPointSDL, (SDL_RendererFlip)flip);
    CHECK_SDL_ERROR();
}

void Renderer::DrawTexture(DrawableTexture& texture, glm::ivec2 position,
                           glm::vec2 scale, f32 rotation, Color tint,
                           CenterPoint anchor, CenterPoint rotationCenter) {
    DrawTextureInternal(texture.Finalize(*this), position, scale, rotation,
                        tint, anchor, rotationCenter);
}

void Renderer::DrawTextureRect(DrawableTexture& texture,
                               const SDL_Rect& sourceRect, glm::ivec2 position,
                               glm::vec2 scale, f32 rotation, Color tint,
                               CenterPoint anchor, CenterPoint rotationCenter) {
    DrawTextureInternal(texture.Finalize(*this), position, scale, rotation,
                        tint, anchor, rotationCenter, &sourceRect);
}

void Renderer::DrawText(Label& label, glm::ivec2 position, glm::vec2 scale,
                        f32 rotation, Color tint, CenterPoint anchor,
                        CenterPoint rotationCenter) {
    if (label.m_Texture == nullptr) {
        auto* surface = TTF_RenderText_Blended(
            label.m_Font, label.m_Content.c_str(), label.m_Color);
        CHECK_SDL_ERROR();

        label.m_Texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
        CHECK_SDL_ERROR();

        SDL_FreeSurface(surface);
        CHECK_SDL_ERROR();
    }

    ASSERT(label.m_Texture != nullptr);

    DrawTextureInternal(label.m_Texture, position, scale, rotation, tint,
                        anchor, rotationCenter);
}

}  // namespace GUI