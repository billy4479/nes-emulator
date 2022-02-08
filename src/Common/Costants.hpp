#pragma once

#include <glm/ext/vector_int2.hpp>
#include <string_view>

#include "Common.hpp"

constexpr glm::ivec2 NES_SCREEN_SIZE = {256, 240};
constexpr i32 PIXEL_SCALE_FACTOR = 4;
constexpr glm::ivec2 NES_EMULATOR_SCREEN_SIZE =
    NES_SCREEN_SIZE * PIXEL_SCALE_FACTOR;  // 1024x960
constexpr glm::ivec2 EMULATOR_SIZE = {NES_EMULATOR_SCREEN_SIZE.x + 750,
                                      NES_SCREEN_SIZE.y* PIXEL_SCALE_FACTOR};
constexpr struct {
    i32 fontSize;
    i32 charWidth;
    i32 charHeight;
    std::string_view path;
    std::string_view name;
} DEBUG_FONT = {
    .fontSize = 20,
    .charWidth = DEBUG_FONT.fontSize / 2 + 1,
    .charHeight = DEBUG_FONT.fontSize + 2,
    .path = "JetBrains Mono Regular Nerd Font Complete Mono.ttf",
    .name = "DEBUG_FONT",
};