#include "Application.hpp"

Application::Application(i32 width, i32 height)
    : m_Height(height), m_Width(width) {
    if (SDL_Init(SDL_INIT_EVERYTHING))
        throw std::runtime_error("SDL failed to initialize.");

    m_Window = SDL_CreateWindow("NES-Emulator", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, m_Width, m_Height, 0);

    if (m_Window == nullptr)
        std::runtime_error("SDL failed to create a window.");

    m_Renderer.Init(m_Window, m_Width, m_Height);
}

Application::~Application() {
    SDL_DestroyWindow(m_Window);
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
    while (isRunning) {
        frameStart = SDL_GetTicks();

        m_EventHandler.HandleEvents();

        // Logic here
        m_Renderer.PutPixel(count++, line, Color(0, 153, 0));
        if (count >= m_Width) {
            line++;
            count = 0;
        }
        // End of logic

        m_Renderer.Draw();

        isRunning = !m_EventHandler.ShouldClose();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }
}

void Application::AskToStop() { isRunning = false; }