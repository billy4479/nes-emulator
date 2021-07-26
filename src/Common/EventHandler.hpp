#pragma once

#include "Common.hpp"
#include <SDL2/SDL.h>

class EventHandler {
  public:
    void HandleEvents();
    bool ShouldClose();

  private:
    bool m_ShouldClose = false;
};