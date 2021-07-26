#pragma once

#include "Common/AssetManager.hpp"
#include "Common/Common.hpp"
#include "Common/EventHandler.hpp"
#include "Emulation/NES.hpp"
#include "Gui/Renderer.hpp"
#include <SDL2/SDL.h>
#include <stdexcept>

class Application {

  public:
    Application(i32 width, i32 height);
    ~Application();

    void Run();
    Gui::Renderer *GetRenderer();
    AssetManager *GetAssetManager();

  private:
    bool isRunning = false;
    void AskToStop();

    static constexpr i32 FPS = 60;
    static constexpr i32 frameDelay = 1000 / FPS;

    i32 m_Height, m_Width;

    SDL_Window *m_Window;
    AssetManager m_AssetManager;
    Gui::Renderer m_Renderer;
    EventHandler m_EventHandler;
    Emulation::NES m_Nes;
};