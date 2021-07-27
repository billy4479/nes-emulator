
#include "PPU.hpp"

namespace Emulation {

u8 PPU::CPURead(u16 addr, bool readOnly) {
    u8 data = 0x00;

    switch (addr) {
        case 0x0000:  // Control
            break;
        case 0x0001:  // Mask
            break;
        case 0x0002:  // Status
            break;
        case 0x0003:  // OAM Address
            break;
        case 0x0004:  // OAM Data
            break;
        case 0x0005:  // Scroll
            break;
        case 0x0006:  // PPU Address
            break;
        case 0x0007:  // PPU Data
            break;
    }

    return data;
}

void PPU::CPUWrite(u16 addr, u8 data) {
    switch (addr) {
        case 0x0000:  // Control
            break;
        case 0x0001:  // Mask
            break;
        case 0x0002:  // Status
            break;
        case 0x0003:  // OAM Address
            break;
        case 0x0004:  // OAM Data
            break;
        case 0x0005:  // Scroll
            break;
        case 0x0006:  // PPU Address
            break;
        case 0x0007:  // PPU Data
            break;
    }
}

u8 PPU::PPURead(u16 addr, bool readOnly) {
    u8 data = 0;
    addr &= 0x3FFF;

    if (m_Cartridge->PPURead(addr, data)) {
    }

    return data;
}

void PPU::PPUWrite(u16 addr, u8 data) {
    addr &= 0x3FFF;
    if (m_Cartridge->PPUWrite(addr, data)) {
    }
}

void PPU::ConnectCatridge(const std::shared_ptr<Cartridge> &cartridge) {
    m_Cartridge = cartridge;
}

void PPU::Clock() {
    m_Cycle++;
    if (m_Cycle >= 341) {
        m_Cycle = 0;
        m_ScanLine++;
        if (m_ScanLine >= 261) {
            m_ScanLine = -1;
            m_FrameComplete = true;
        }
    }
}

}  // namespace Emulation