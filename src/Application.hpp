#pragma once

#include <SDL2/SDL.h>

#include <glm/ext/vector_int2.hpp>

#include "Common/AssetManager.hpp"
#include "Common/Common.hpp"
#include "Common/EventHandler.hpp"
#include "Emulation/Bus.hpp"
#include "Gui/FPSManager.hpp"
#include "Gui/Renderer.hpp"

class Application {
   public:
    Application(glm::ivec2);
    ~Application();

    void Run();
    GUI::Renderer *GetRenderer();
    AssetManager *GetAssetManager();

   private:
    bool isRunning = false;
    void AskToStop();

    static constexpr i32 FPS = 60;
    static constexpr i32 frameDelay = 1000 / FPS;

    AssetManager m_AssetManager;
    GUI::Renderer m_Renderer;
    FPSManager m_FPSManager;
    EventHandler m_EventHandler;
    Emulation::Bus m_Nes;
};