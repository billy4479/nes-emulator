#include "Label.hpp"

#include <SDL_render.h>

namespace GUI {

Label::Label(const std::string& text, TTF_Font* font, const Color& color)
    : m_Content(text), m_Font(font), m_Color(color) {}

void Label::SetText(const std::string& text) {
    m_Content = text;
    SDL_DestroyTexture(m_Texture);
    m_Texture = nullptr;
}

void Label::SetFont(TTF_Font* font) {
    m_Font = font;
    SDL_DestroyTexture(m_Texture);
    m_Texture = nullptr;
}

void Label::SetColor(const Color& color) {
    m_Color = color;
    SDL_DestroyTexture(m_Texture);
    m_Texture = nullptr;
}

const std::string& Label::GetText() const { return m_Content; }
}  // namespace GUI