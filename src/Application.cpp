#include "Application.hpp"

#include <SDL2/SDL_ttf.h>

#include <cstdio>
#include <memory>

#include "Common/Common.hpp"
#include "Common/Costants.hpp"
#include "Emulation/CPU.hpp"
#include "Gui/Label.hpp"
#include "glm/ext/vector_int2.hpp"

Application::Application(glm::ivec2 size)
    : m_AssetManager("assets"), m_Renderer("NES-Emulator", size) {
    assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);
    assert(TTF_Init() == 0);

    m_AssetManager.LoadFont(
        "JetBrains Mono Regular Nerd Font Complete Mono.ttf", "JetBrains Mono",
        32);

    // dbg_print("%s", m_Nes.DumpAsHex(0x8000, 0x00F0).c_str());
}

Application::~Application() {
    m_AssetManager.ReleaseSDLDependencies();
    TTF_Quit();
    SDL_Quit();
}

void Application::Run() {
    assert(!isRunning);
    isRunning = true;

    m_Nes.LoadAndInsertCartridge("nestest.nes", m_AssetManager);
    m_Nes.Reset();

    auto scale = glm::ivec2{1, 1} * PIXEL_SCALE_FACTOR;

    auto fpsLabel = GUI::Label("", m_AssetManager.GetFont("JetBrains Mono"));

    while (isRunning) {
        m_FPSManager.StartFrame();

        m_Renderer.Clear();
        m_EventHandler.HandleEvents();

        // Logic here

        do {
            m_Nes.Clock();
        } while (!m_Nes.ppu.m_FrameComplete);

        m_Nes.ppu.m_FrameComplete = false;
        m_Renderer.DrawTexture(m_Nes.ppu.GetScreenTexture(), {0, 0}, scale);

        {
            std::stringstream ss;
            ss << "FPS: " << m_FPSManager.GetActualFPS();
            fpsLabel.SetText(ss.str());
            m_Renderer.DrawText(fpsLabel, {NES_EMULATOR_SCREEN_SIZE.x + 10, 0});
        }

        // End of logic

        m_Renderer.RenderToScreen();

        isRunning = !m_EventHandler.ShouldClose();

        m_FPSManager.EndFrameAndWait();
    }
}

void Application::AskToStop() { isRunning = false; }

GUI::Renderer *Application::GetRenderer() { return &m_Renderer; }

AssetManager *Application::GetAssetManager() { return &m_AssetManager; }
