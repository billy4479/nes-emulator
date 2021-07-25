#pragma once

#include "../Application.hpp"
#include "../Common.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

namespace Rendering {

    class Label {
      public:
        Label(Application *, const std::string &, const std::string &,
              std::pair<i32, i32>);
        ~Label();

        void SetText(const std::string &);

      private:
        std::string m_font_name;
        std::string m_text;
        SDL_Rect m_position;
        SDL_Texture *m_texture = nullptr;
        Application *m_Application = nullptr;
    };
} // namespace Rendering