#pragma once

#include <array>
#include <memory>
#include <stdexcept>
#include <string>

#include "../Common/AssetManager.hpp"
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
    CPU m_CPU;
    PPU m_PPU;

   private:
    std::array<u8, 2048> m_CPURam;  // This is going to change
    std::shared_ptr<Cartridge> m_Cartridge;

   public:
    /*  *** Read & Write ***  */
    void CPUWrite(u16 addr, u8 data);
    u8 CPURead(u16 addr, bool readOnly = false);

    /*  *** System Interface ***  */
    void InsertCartridge(const std::shared_ptr<Cartridge> &cartridge);
    void LoadAndInsertCartridge(std::filesystem::path, const AssetManager &);
    void Reset();
    void Clock();

    std::string DumpAsHex(u16 start, u16 length);

   private:
    u32 m_ClockCounter;
};

}  // namespace Emulation