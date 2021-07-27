#pragma once
#include <memory>

#include "../Common/Common.hpp"
#include "Cartridge.hpp"

namespace Emulation {

class PPU {
   public:
    PPU() = default;
    ~PPU() = default;

    u8 CPURead(u16 addr, bool readOnly = false);
    void CPUWrite(u16 addr, u8 data);

    u8 PPURead(u16 addr, bool readOnly = false);
    void PPUWrite(u16 addr, u8 data);

    void ConnectCatridge(const std::shared_ptr<Cartridge> &);
    void Clock();

   private:
    std::shared_ptr<Cartridge> m_Cartridge;

    u8 m_NameTable[2][1024];
    u8 m_Palettes[32];
    // u8 m_Pattern[2][4096];

    u16 m_Cycle = 0;
    u16 m_ScanLine = 0;
    bool m_FrameComplete = false;
};

}  // namespace Emulation