#include "EventHandler.hpp"

void EventHandler::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        for (auto [id, l] : m_Listeners) {
            if (l.Type == event.type) {
                l.Do(event);
            }
        }
    }
}

bool EventHandler::ShouldClose() { return m_ShouldClose; }

u32 EventHandler::AddListener(Listener l) {
    m_Listeners[++m_NextID] = l;
    return m_NextID;
}

void EventHandler::RemoveListener(u32 id) { m_Listeners.erase(id); }

void EventHandler::RemoveAllListeners() { m_Listeners.clear(); }