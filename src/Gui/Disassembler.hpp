#pragma once

#include <cstdio>

#include "../Common/EventHandler.hpp"
#include "../Emulation/Bus.hpp"
#include "DrawableTexture.hpp"
#include "Label.hpp"
#include "Renderer.hpp"
#include "glm/ext/vector_int2.hpp"

namespace Emulation {

class Disassembler {
   public:
    Disassembler(Emulation::Bus&, GUI::Renderer&, AssetManager&, EventHandler&);
    void Render();

   private:
    void CPURegisters();
    void CPUStatus();
    void MemoryDisassembly();
    void PPUPalettes();
    void PPUSprites();

   private:
    static constexpr i32 leftPadding = 15;
    static constexpr i32 paletteSize = 16;

    Bus& m_Bus;
    GUI::Renderer& m_Renderer;
    TTF_Font* m_Font;

    glm::ivec2 m_NextPosition;

    struct {
        struct {
            GUI::Label StatusLabel;
            GUI::Label Bits[8];
        } Status;

        struct {
            GUI::Label Name;
            GUI::Label Value;
        } Registers[5];
    } m_CPURegisters;

    std::map<u16, GUI::Label> m_DisassembledCode;

    struct P {
        struct Palette {
            Palette()
                : Colors{
                      GUI::DrawableTexture({paletteSize, paletteSize},
                                           Color::white),
                      GUI::DrawableTexture({paletteSize, paletteSize},
                                           Color::white),
                      GUI::DrawableTexture({paletteSize, paletteSize},
                                           Color::white),
                      GUI::DrawableTexture({paletteSize, paletteSize},
                                           Color::white),
                  } {}
            GUI::DrawableTexture Colors[4];
        };

        Palette Palettes[8];
        struct {
            u8 ID;
            GUI::DrawableTexture Selected;
        } Selected = {0, GUI::DrawableTexture({4 * 16, 2}, Color::red)};
    };

    P m_Palettes;

   private:
    void Disassemble(u16 start, u16 end);
    void NextLine();
};
}  // namespace Emulation