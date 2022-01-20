#pragma once

#include <glm/ext/vector_int2.hpp>

#include "Common.hpp"

constexpr glm::ivec2 NES_SCREEN_SIZE = {256, 240};
constexpr i32 PIXEL_SCALE_FACTOR = 4;
constexpr glm::ivec2 NES_EMULATOR_SCREEN_SIZE =
    NES_SCREEN_SIZE * PIXEL_SCALE_FACTOR;  // 1024x960
constexpr glm::ivec2 EMULATOR_SIZE = {NES_EMULATOR_SCREEN_SIZE.x + 300, 960};