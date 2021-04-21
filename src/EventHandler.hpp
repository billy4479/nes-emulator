#pragma once

#include <SDL2/SDL.h>
#include <iostream>

class EventHandler {
  public:
    void HandleEvents();
    bool ShouldClose();

  private:
    bool m_ShouldClose = false;
};