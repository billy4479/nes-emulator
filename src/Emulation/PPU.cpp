
#include "PPU.hpp"

#include <cstdlib>

#include "../Common/Costants.hpp"

namespace Emulation {

PPU::PPU() : m_Screen(NES_SCREEN_SIZE) {
    // Set color palette
    {
        m_ColorPalette[0] = {84, 84, 84};
        m_ColorPalette[1] = {0, 30, 116};
        m_ColorPalette[2] = {8, 16, 144};
        m_ColorPalette[3] = {48, 0, 136};
        m_ColorPalette[4] = {68, 0, 100};
        m_ColorPalette[5] = {92, 0, 48};
        m_ColorPalette[6] = {84, 4, 0};
        m_ColorPalette[7] = {60, 24, 0};
        m_ColorPalette[8] = {32, 42, 0};
        m_ColorPalette[9] = {8, 58, 0};
        m_ColorPalette[10] = {0, 64, 0};
        m_ColorPalette[11] = {0, 60, 0};
        m_ColorPalette[12] = {0, 50, 60};
        m_ColorPalette[13] = {0, 0, 0};
        m_ColorPalette[14] = {0, 0, 0};
        m_ColorPalette[15] = {0, 0, 0};

        m_ColorPalette[16] = {152, 150, 152};
        m_ColorPalette[17] = {8, 76, 196};
        m_ColorPalette[18] = {48, 50, 236};
        m_ColorPalette[19] = {92, 30, 228};
        m_ColorPalette[20] = {136, 20, 176};
        m_ColorPalette[21] = {160, 20, 100};
        m_ColorPalette[22] = {152, 34, 32};
        m_ColorPalette[23] = {120, 60, 0};
        m_ColorPalette[24] = {84, 90, 0};
        m_ColorPalette[25] = {40, 114, 0};
        m_ColorPalette[26] = {8, 124, 0};
        m_ColorPalette[27] = {0, 118, 40};
        m_ColorPalette[28] = {0, 102, 120};
        m_ColorPalette[29] = {0, 0, 0};
        m_ColorPalette[30] = {0, 0, 0};
        m_ColorPalette[31] = {0, 0, 0};

        m_ColorPalette[32] = {236, 238, 236};
        m_ColorPalette[33] = {76, 154, 236};
        m_ColorPalette[34] = {120, 124, 236};
        m_ColorPalette[35] = {176, 98, 236};
        m_ColorPalette[36] = {228, 84, 236};
        m_ColorPalette[37] = {236, 88, 180};
        m_ColorPalette[38] = {236, 106, 100};
        m_ColorPalette[39] = {212, 136, 32};
        m_ColorPalette[40] = {160, 170, 0};
        m_ColorPalette[41] = {116, 196, 0};
        m_ColorPalette[42] = {76, 208, 32};
        m_ColorPalette[43] = {56, 204, 108};
        m_ColorPalette[44] = {56, 180, 204};
        m_ColorPalette[45] = {60, 60, 60};
        m_ColorPalette[46] = {0, 0, 0};
        m_ColorPalette[47] = {0, 0, 0};

        m_ColorPalette[48] = {236, 238, 236};
        m_ColorPalette[49] = {168, 204, 236};
        m_ColorPalette[50] = {188, 188, 236};
        m_ColorPalette[51] = {212, 178, 236};
        m_ColorPalette[52] = {236, 174, 236};
        m_ColorPalette[53] = {236, 174, 212};
        m_ColorPalette[54] = {236, 180, 176};
        m_ColorPalette[55] = {228, 196, 144};
        m_ColorPalette[56] = {204, 210, 120};
        m_ColorPalette[57] = {180, 222, 120};
        m_ColorPalette[58] = {168, 226, 144};
        m_ColorPalette[59] = {152, 226, 180};
        m_ColorPalette[60] = {160, 214, 228};
        m_ColorPalette[61] = {160, 162, 160};
        m_ColorPalette[62] = {0, 0, 0};
        m_ColorPalette[63] = {0, 0, 0};
    }
}

u8 PPU::CPURead(u16 addr, [[maybe_unused]] bool readOnly) const {
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

void PPU::CPUWrite(u16 addr, [[maybe_unused]] u8 data) {
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

u8 PPU::PPURead(u16 addr, [[maybe_unused]] bool readOnly) const {
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

GUI::DrawableTexture& PPU::GetScreenTexture() { return m_Screen; }

void PPU::ConnectCatridge(const std::shared_ptr<Cartridge>& cartridge) {
    m_Cartridge = cartridge;
}

void PPU::Clock() {
    // That's the blank period
    if (!(m_Cycle > NES_SCREEN_SIZE.x || m_ScanLine > NES_SCREEN_SIZE.y)) {
        m_Screen.PutPixel({m_Cycle - 1, m_ScanLine},
                          m_ColorPalette[(rand() % 2) ? 0x3F : 0x30]);
    }

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