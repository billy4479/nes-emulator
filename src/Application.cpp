#include "Application.hpp"

#include <SDL2/SDL_ttf.h>

#include "Common/Common.hpp"
#include "Gui/Label.hpp"
#include "glm/ext/vector_int2.hpp"

Application::Application(glm::ivec2 size) : m_Renderer("NES-Emulator", size) {
    if (SDL_Init(SDL_INIT_EVERYTHING))
        throw std::runtime_error("SDL failed to initialize.");
    if (TTF_Init()) throw std::runtime_error("SDL_ttf failed to initialize.");

    m_AssetManager.LoadFont(
        "JetBrains Mono Regular Nerd Font Complete Mono.ttf", "JetBrains Mono",
        32);

    // dbg_print("%s", m_Nes.DumpAsHex(0x8000, 0x00F0).c_str());
}

Application::~Application() {
    m_AssetManager.ReleaseTTF();
    TTF_Quit();
    SDL_Quit();
}

void Application::Run() {
    assert(!isRunning);
    isRunning = true;

    m_Renderer.Clear();

    u32 frameStart;
    i32 frameTime;

    // i32 count = 0;
    // i32 line = 0;

    GUI::Label l("Test .-.", m_AssetManager.GetFont("JetBrains Mono"));
    u16 h = 0;
    // auto fg = Color(0, 153, 0);
    while (isRunning) {
        m_Renderer.Clear();

        frameStart = SDL_GetTicks();

        m_EventHandler.HandleEvents();

        // Logic here

        m_Renderer.GetEmulatorScreen().Clear(Color::hsl(h++, 0.5, 0.5));
        dbg_print("%d\n", h);

        // for (i32 i = 0; i < m_Width; i++)
        //     m_Renderer.PutPixel(i, count, fg);
        // count++;
        // if (count >= m_Height) {
        //     count = 0;
        //     fg = Color::hsl(180, 100, 100);
        // }

        // m_Renderer.PutPixel(count++, line, fg);
        // if (count >= m_Width) {
        //     line++;
        //     count = 0;
        // }

        // End of logic

        m_Renderer.RenderToScreen();

        isRunning = !m_EventHandler.ShouldClose();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    }
}

void Application::AskToStop() { isRunning = false; }

GUI::Renderer *Application::GetRenderer() { return &m_Renderer; }

AssetManager *Application::GetAssetManager() { return &m_AssetManager; }
