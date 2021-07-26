#include "Application.hpp"
#include "Common/Common.hpp"
#include "Gui/Label.hpp"
#include <SDL2/SDL_ttf.h>

Application::Application(i32 width, i32 height)
    : m_Height(height), m_Width(width) {
    if (SDL_Init(SDL_INIT_EVERYTHING))
        throw std::runtime_error("SDL failed to initialize.");
    if (TTF_Init())
        throw std::runtime_error("SDL_ttf failed to initialize.");

    m_Window = SDL_CreateWindow("NES-Emulator", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, m_Width, m_Height, 0);

    if (m_Window == nullptr)
        std::runtime_error("SDL failed to create a window.");

    m_Renderer.Init(m_Window, m_Width, m_Height);
    m_AssetManager.LoadFont(
        "JetBrains Mono Regular Nerd Font Complete Mono.ttf", "JetBrains Mono",
        32);

    dbg_print("%s", m_Nes.DumpRamAsHex(0x8000, 0x00F0).c_str());
}

Application::~Application() {
    SDL_DestroyWindow(m_Window);
    m_AssetManager.ReleaseTTF();
    TTF_Quit();
    SDL_Quit();
}

void Application::Run() {
    assert(!isRunning);
    isRunning = true;

    m_Renderer.Clear(Color(41, 41, 41));

    u32 frameStart;
    i32 frameTime;

    i32 count = 0;
    i32 line = 0;

    Gui::Label l(this, "JetBrains Mono", "Test .-", {100, 100});
    auto fg = Color(0, 153, 0);
    while (isRunning) {
        frameStart = SDL_GetTicks();

        m_EventHandler.HandleEvents();

        // Logic here

        for (i32 i = 0; i < m_Width; i++)
            m_Renderer.PutPixel(i, count, fg);
        count++;
        if (count >= m_Height) {
            count = 0;
            fg.r++;
        }

        // m_Renderer.PutPixel(count++, line, fg);
        // if (count >= m_Width) {
        //     line++;
        //     count = 0;
        // }

        // End of logic

        m_Renderer.Draw();

        isRunning = !m_EventHandler.ShouldClose();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }
}

void Application::AskToStop() { isRunning = false; }

Gui::Renderer *Application::GetRenderer() { return &m_Renderer; }

AssetManager *Application::GetAssetManager() { return &m_AssetManager; }
