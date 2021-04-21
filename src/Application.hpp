#pragma once
#include "EventHandler.hpp"
#include "Renderer.hpp"
#include "Types.hpp"
#include <SDL2/SDL.h>
#include <stdexcept>

class Application {

  public:
    Application(i32 width, i32 height);
    ~Application();

    void Run();

  private:
    bool isRunning = false;
    void AskToStop();

    static constexpr i32 FPS = 60;
    static constexpr i32 frameDelay = 1000 / FPS;

    i32 m_Height, m_Width;

    SDL_Window *m_Window;
    Renderer m_Renderer;
    EventHandler m_EventHandler;
};