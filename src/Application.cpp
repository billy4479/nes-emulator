#include "Application.hpp"

#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL_events.h>
#include <SDL_scancode.h>

#include <cstdio>
#include <memory>

#include "Common/Common.hpp"
#include "Common/Costants.hpp"
#include "Emulation/CPU.hpp"
#include "Gui/Disassembler.hpp"
#include "Gui/Label.hpp"
#include "glm/ext/vector_int2.hpp"

Application::Application(glm::ivec2 size)
    : m_AssetManager("assets"), m_Renderer("NES-Emulator", size) {
    m_AssetManager.LoadFont(
        "JetBrains Mono Regular Nerd Font Complete Mono.ttf", "JetBrains Mono",
        28);

    // dbg_print("%s", m_Nes.DumpAsHex(0x8000, 0x00F0).c_str());
}

Application::~Application() {
    m_AssetManager.ReleaseSDLDependencies();
    TTF_Quit();
    SDL_Quit();
}

void Application::Run() {
    ASSERT(!isRunning);
    isRunning = true;

    m_Nes.LoadAndInsertCartridge("nestest.nes", m_AssetManager);
    m_Nes.Reset();

    m_EventHandler.RemoveAllListeners();
    m_EventHandler.AddListener(Listener{
        .Type = SDL_QUIT,
        .Do =
            [&](auto) {
                dbg_print("Quitting\n");
                isRunning = false;
            },
    });

    bool stepByStep = true;
    bool stepClock = false;
    bool stepFrame = false;

    m_EventHandler.AddListener(Listener{
        .Type = SDL_KEYDOWN,
        .Do =
            [&](SDL_Event &e) {
                switch (e.key.keysym.scancode) {
                    case SDL_SCANCODE_SPACE:
                        stepByStep = !stepByStep;
                        break;
                    case SDL_SCANCODE_C:
                        stepClock = true;
                        break;
                    case SDL_SCANCODE_F:
                        stepFrame = true;
                        break;
                    default:
                        break;
                }
            },
    });

    auto disassembler =
        Emulation::Disassembler(m_Nes, m_Renderer, m_AssetManager);

    auto scale = glm::ivec2{1, 1} * PIXEL_SCALE_FACTOR;

    while (isRunning) {
        m_FPSManager.StartFrame();
        m_Renderer.Clear();
        m_EventHandler.HandleEvents();

        // Logic here

        if (stepByStep) {
            if (stepClock) {
                do {
                    m_Nes.Clock();
                } while (!m_Nes.m_CPU.IsComplete());

                // Do I really need this?
                do {
                    m_Nes.Clock();
                } while (m_Nes.m_CPU.IsComplete());
                stepClock = false;
            }

            if (stepFrame) {
                do {
                    m_Nes.Clock();
                } while (!m_Nes.m_PPU.m_FrameComplete);
                do {
                    m_Nes.Clock();
                } while (!m_Nes.m_CPU.IsComplete());
                m_Nes.m_PPU.m_FrameComplete = false;
                stepFrame = false;
            }
        } else {
            do {
                m_Nes.Clock();
            } while (!m_Nes.m_PPU.m_FrameComplete);
        }

        m_Nes.m_PPU.m_FrameComplete = false;
        m_Renderer.DrawTexture(m_Nes.m_PPU.GetScreenTexture(), {0, 0}, scale);

        disassembler.Render();

        // End of logic

        m_Renderer.RenderToScreen();
        m_FPSManager.EndFrameAndWait();
    }
}

void Application::AskToStop() { isRunning = false; }

GUI::Renderer *Application::GetRenderer() { return &m_Renderer; }

AssetManager *Application::GetAssetManager() { return &m_AssetManager; }
