#include "Renderer.hpp"

namespace Rendering {

    void Renderer::Init(SDL_Window *window, i32 w, i32 h) {
        assert(m_Renderer == nullptr);
        m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (m_Renderer == nullptr)
            std::runtime_error("SDL failed to initialize renderer.");

        m_BackBuffer.Init(w, h, *this);
    }

    Renderer::~Renderer() { SDL_DestroyRenderer(*this); }

    void Renderer::PutPixel(i32 x, i32 y, Color c) {
        assert(m_Renderer != nullptr);
        m_BackBuffer.At(x, y) = c;
        return;
    }

    void Renderer::Clear(Color c) { m_BackBuffer.Clear(c); }

    void Renderer::Draw() { m_BackBuffer.Swap(); }
} // namespace Rendering