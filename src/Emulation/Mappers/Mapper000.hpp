#pragma once
#include "Mapper.hpp"

namespace Emulation {
namespace Mappers {

class Mapper000 : public Mapper {
   public:
    Mapper000(u8 prgBanks, u8 chrBanks) : Mapper(prgBanks, chrBanks) {}
    ~Mapper000() = default;

    bool CPUMapRead(u16 addr, u32 &mappedAddr) override {
        if (addr >= 0x8000) {
            mappedAddr = addr & (m_PRGBanks > 1 ? 0x7FFF : 0x3FFF);
            return true;
        }

        return false;
    }

    bool CPUMapWrite(u16 addr, u32 &mappedAddr) override {
        if (addr >= 0x8000) {
            mappedAddr = addr & (m_PRGBanks > 1 ? 0x7FFF : 0x3FFF);
            return true;
        }

        return false;
    }

    bool PPUMapRead(u16 addr, u32 &mappedAddr) override {
        if (addr <= 0x1FFF) {
            mappedAddr = addr;
            return true;
        }

        return false;
    }

    bool PPUMapWrite(u16 addr, u32 &mappedAddr) override {
        if (addr <= 0x1FFF) {
            // if (nCHRBanks == 0) {
            // Treat as RAM
            mappedAddr = addr;
            return true;
            // }
        }

        return false;
    }
};

}  // namespace Mappers
}  // namespace Emulation