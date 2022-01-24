#pragma once

#include <SDL2/SDL.h>
#include <SDL_events.h>

#include <functional>
#include <unordered_map>
#include <vector>

#include "Common.hpp"

struct Listener {
    SDL_EventType Type;
    std::function<void(SDL_Event&)> Do;
};

class EventHandler {
   public:
    void HandleEvents();
    bool ShouldClose();

    u32 AddListener(Listener l);
    void RemoveListener(u32);
    void RemoveAllListeners();

   private:
    bool m_ShouldClose = false;
    u32 m_NextID = 0;
    std::unordered_map<u32, Listener> m_Listeners;
};