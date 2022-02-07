#pragma once

#include <SDL2/SDL.h>
#include <SDL_rect.h>

#include <glm/ext/vector_int2.hpp>
#include <glm/vec2.hpp>
#include <string_view>

#include "../Common/Color.hpp"
#include "../Common/Common.hpp"
#include "DrawableTexture.hpp"
#include "Label.hpp"

namespace GUI {

enum class CenterPoint {
    TOP_RIGHT,
    TOP_CENTER,
    TOP_LEFT,
    CENTER_RIGHT,
    CENTER_CENTER,
    CENTER_LEFT,
    BOTTOM_RIGHT,
    BOTTOM_CENTER,
    BOTTOM_LEFT,
};

class Renderer {
   public:
    Renderer(std::string_view name, glm::ivec2 size);
    ~Renderer();

    void Clear();
    void RenderToScreen();
    void DrawTexture(DrawableTexture &texture, glm::ivec2 position,
                     glm::vec2 scale = {1, 1}, f32 rotation = 0,
                     Color tint = Color::white,
                     CenterPoint anchor = CenterPoint::TOP_LEFT,
                     CenterPoint rotationCenter = CenterPoint::CENTER_CENTER);

    void DrawTextureRect(
        DrawableTexture &texture, const SDL_Rect &sourceRect,
        glm::ivec2 position, glm::vec2 scale = {1, 1}, f32 rotation = 0,
        Color tint = Color::white, CenterPoint anchor = CenterPoint::TOP_LEFT,
        CenterPoint rotationCenter = CenterPoint::CENTER_CENTER);

    void DrawText(Label &label, glm::ivec2 position, glm::vec2 scale = {1, 1},
                  f32 rotation = 0, Color tint = Color::white,
                  CenterPoint anchor = CenterPoint::TOP_LEFT,
                  CenterPoint rotationCenter = CenterPoint::CENTER_CENTER);

   private:
    void DrawTextureInternal(SDL_Texture *texture, glm::ivec2 position,
                             glm::vec2 scale, f32 rotation, Color tint,
                             CenterPoint anchor, CenterPoint rotationCenter,
                             const SDL_Rect *sourceRect = nullptr);

   private:
    SDL_Renderer *m_Renderer = nullptr;
    SDL_Window *m_Window = nullptr;

    SDL_Texture *Convert(SDL_Surface *surface);
    friend class DrawableTexture;
};

}  // namespace GUI