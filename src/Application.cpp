#include "Application.hpp"

#include <SDL2/SDL_ttf.h>

#include <memory>

#include "Common/Common.hpp"
#include "Common/Costants.hpp"
#include "Emulation/CPU.hpp"
#include "Gui/Label.hpp"
#include "glm/ext/vector_int2.hpp"

Application::Application(glm::ivec2 size) : m_Renderer("NES-Emulator", size) {
    assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);
    assert(TTF_Init() == 0);

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

    m_Nes.InsertCartridge(std::make_shared<Emulation::Cartridge>(
        "/home/billy/code/nes-emulator/roms/nestest.nes"));
    m_Nes.Reset();

    u32 frameStart;
    i32 frameTime;

    auto scale = glm::ivec2{1, 1} * PIXEL_SCALE_FACTOR;

    while (isRunning) {
        frameStart = SDL_GetTicks();

        m_Renderer.Clear();
        m_EventHandler.HandleEvents();

        // Logic here

        do {
            m_Nes.Clock();
        } while (!m_Nes.ppu.m_FrameComplete);

        m_Nes.ppu.m_FrameComplete = false;
        m_Renderer.DrawTexture(m_Nes.ppu.GetScreenTexture(), {0, 0}, scale);

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
