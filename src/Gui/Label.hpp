#pragma once

#include <SDL2/SDL_ttf.h>

#include <string>

#include "../Common/Color.hpp"
#include "../Common/Common.hpp"

namespace GUI {

class Label {
   public:
    Label() = default;
    Label(const std::string& text, TTF_Font* font,
          const Color& color = Color::white);

    void SetText(const std::string& text);
    void SetFont(TTF_Font*);
    void SetColor(const Color& color);

    const std::string& GetText() const;

   private:
    std::string m_Content;
    TTF_Font* m_Font = nullptr;
    SDL_Texture* m_Texture = nullptr;
    Color m_Color = Color::white;

    friend class Renderer;
};

}  // namespace GUI