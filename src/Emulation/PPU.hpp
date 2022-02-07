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

   private:
    std::shared_ptr<Cartridge> m_Cartridge;

    u8 m_NameTable[2][1024];
    u8 m_PaletteTable[32];
    // u8 m_Pattern[2][4096];

    Color m_ColorPalette[0x40];
    GUI::DrawableTexture m_Screen;

   public:
    GUI::DrawableTexture& GetScreenTexture();
    bool m_FrameComplete = false;

   private:
    u16 m_Cycle = 0;
    u16 m_ScanLine = 0;
};

}  // namespace Emulation