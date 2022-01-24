#include "Label.hpp"

#include <SDL_render.h>

namespace GUI {

Label::Label(const std::string& text, TTF_Font* font, const Color& color)
    : m_Content(text), m_Font(font), m_Color(color) {}

Label::~Label() {
    if (m_Texture != nullptr) SDL_DestroyTexture(m_Texture);
    m_Texture = nullptr;
}

void Label::SetText(const std::string& text) {
    if (m_Content == text) return;
    m_Content = text;
    if (m_Texture != nullptr) SDL_DestroyTexture(m_Texture);
    m_Texture = nullptr;
}

void Label::SetFont(TTF_Font* font) {
    if (m_Font == font) return;
    m_Font = font;
    if (m_Texture != nullptr) SDL_DestroyTexture(m_Texture);
    m_Texture = nullptr;
}

void Label::SetColor(const Color& color) {
    if (m_Color == color) return;
    m_Color = color;
    if (m_Texture != nullptr) SDL_DestroyTexture(m_Texture);
    m_Texture = nullptr;
}

const std::string& Label::GetText() const { return m_Content; }
}  // namespace GUI