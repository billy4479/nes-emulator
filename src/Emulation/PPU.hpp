#pragma once
#include <memory>

#include "../Common/Color.hpp"
#include "../Common/Common.hpp"
#include "../Gui/DrawableTexture.hpp"
#include "Cartridge.hpp"

namespace Emulation {

class PPU {
   public:
    PPU();
    ~PPU() = default;

    u8 CPURead(u16 addr, bool readOnly = false) const;
    void CPUWrite(u16 addr, u8 data);

    u8 PPURead(u16 addr, bool readOnly = false) const;
    void PPUWrite(u16 addr, u8 data);

    void ConnectCatridge(const std::shared_ptr<Cartridge>&);
    void Clock();

    bool IsBlankPeriod();
    bool FrameComplete = false;

    GUI::DrawableTexture& GetScreenTexture();

   private:
    void UpdatePatternTables(u8 palette);
    void GeneratePatternTable(u8 i, u8 palette, GUI::DrawableTexture& dest);
    Color GetColorFromPalette(u8 palette, u8 pixel) const;

   private:
    std::shared_ptr<Cartridge> m_Cartridge;

    // Background layout
    u8 m_NameTables[2][1024];

    // Colors
    // 8 x 4
    u8 m_PaletteMemory[32];

    // Sprites
    // Consider 2 grids 16x16 of 8x8 tiles (total of 256x256 pixels)
    // Each pixel is just 2 bits
    u8 m_PatternTables[2][4096];

    GUI::DrawableTexture m_PatternTableSprites[2];
    Color m_ColorPalette[0x40];

    GUI::DrawableTexture m_Screen;

    u16 m_Cycle = 0;
    u16 m_ScanLine = 0;

    friend class Disassembler;
};

}  // namespace Emulation