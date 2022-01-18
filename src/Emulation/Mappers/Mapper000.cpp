#include "Mapper000.hpp"

namespace Emulation {
namespace Mappers {

bool Mapper000::CPUMapRead(u16 addr, u32 &mappedAddr) {
    if (addr >= 0x8000) {
        mappedAddr = addr & (m_PRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper000::CPUMapWrite(u16 addr, u32 &mappedAddr) {
    if (addr >= 0x8000) {
        mappedAddr = addr & (m_PRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper000::PPUMapRead(u16 addr, u32 &mappedAddr) {
    if (addr <= 0x1FFF) {
        mappedAddr = addr;
        return true;
    }

    return false;
}

bool Mapper000::PPUMapWrite(u16 addr, u32 &mappedAddr) {
    if (addr <= 0x1FFF) {
        if (m_CHRBanks == 0) {
            // Treat as RAM
            mappedAddr = addr;
            return true;
        }
    }

    return false;
}

}  // namespace Mappers
}  // namespace Emulation