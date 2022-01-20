
#include "Bus.hpp"

#include <assert.h>

#include <sstream>

#include "CPU.hpp"

namespace Emulation {

Bus::Bus() {
    // Clear ram to 0;
    m_CPURam.fill(0);
    cpu.ConnectToBus(this);
}

Bus::~Bus() {}

void Bus::CPUWrite(u16 addr, u8 data) {
    if (m_Cartridge->CPUWrite(addr, data)) {
    } else if (addr <= 0x1FFF) {
        // CPU's RAM is from 0x0000 to 0x1FFF
        // but since there are only 2KB of RAM we mirror it.
        m_CPURam[addr & 0x07FF] = data;

    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        // The PPU is from 0x2000 to 0x3FFF
        // but the interface is only 8 byte, so we mirror it.
        ppu.CPUWrite(addr & 0x0007, data);

    } else
        throw std::out_of_range("Specified address is outside the bus' range.");
}
u8 Bus::CPURead(u16 addr, bool readOnly) {
    u8 data = 0;

    if (m_Cartridge->CPURead(addr, data)) {
    }

    else if (addr <= 0x1FFF)
        data = m_CPURam[addr & 0x07FF];

    else if (addr >= 0x2000 && addr <= 0x3FFF)
        data = ppu.CPURead(addr & 0x0007, readOnly);

    else {
        dbg_print("[!] Specified address %x is outside the bus' range.\n",
                  addr);
        // assert(false);
    }
    return data;
}

void Bus::InsertCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    m_Cartridge = cartridge;
    ppu.ConnectCatridge(cartridge);
}

void Bus::LoadAndInsertCartridge(std::filesystem::path path,
                                 const AssetManager& am) {
    InsertCartridge(std::make_shared<Emulation::Cartridge>(am.LoadROM(path)));
}

void Bus::Clock() {
    ppu.Clock();
    if (m_ClockCounter % 3 == 0) cpu.Clock();
    m_ClockCounter++;
}

void Bus::Reset() {
    cpu.Reset();
    m_ClockCounter = 0;
}

std::string Bus::DumpAsHex(u16 start, u16 length) {
    std::stringstream ss;
    assert(start + length < m_CPURam.size());

    auto hex_u8 = [](u8 n) {
        std::string s(2, '0');
        for (int i = 1; i >= 0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    };

    auto hex_u16 = [](u16 n) {
        std::string s(4, '0');
        for (int i = 3; i >= 0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    };

    for (u16 i = start; i < start + length; i++) {
        if (i % 16 == 0) ss << "\n" << hex_u16(i) << ":\t";
        ss << hex_u8(m_CPURam[i]) << " ";
    }

    ss << "\n";

    return ss.str();
}

}  // namespace Emulation