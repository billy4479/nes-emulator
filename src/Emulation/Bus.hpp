#pragma once

#include <array>
#include <memory>
#include <stdexcept>
#include <string>

#include "../Common/Common.hpp"
#include "CPU.hpp"
#include "Cartridge.hpp"
#include "PPU.hpp"

namespace Emulation {

class Bus {
   public:
    Bus();
    ~Bus();

    /*  *** Devices *** */
    CPU cpu;
    PPU ppu;
    std::array<u8, 2048> m_CPURam;  // This is going to change
    std::shared_ptr<Cartridge> m_Cartridge;

    /*  *** Read & Write ***  */
    void CPUWrite(u16 addr, u8 data);
    u8 CPURead(u16 addr, bool readOnly = false);

    /*  *** System Interface ***  */
    void InsertCartridge(const std::shared_ptr<Cartridge> &cartridge);
    void Reset();
    void Clock();

    std::string DumpAsHex(u16 start, u16 length);

   private:
    u32 m_ClockCounter;
};

}  // namespace Emulation