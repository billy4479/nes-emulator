
#include "Bus.hpp"

#include <assert.h>

#include <sstream>

#include "CPU.hpp"

namespace Emulation {

Bus::Bus() {
    // Clear ram to 0;
    for (auto &i : m_CPURam) i = 0x00;

    cpu.ConnectToBus(this);
}

Bus::~Bus() {}

void Bus::CPUWrite(u16 addr, u8 data) {
    if (m_Cartridge->CPUWrite(addr, data)) {
    }

    else if (addr <= 0x1FFF)
        m_CPURam[addr & 0x07FF] = data;

    else if (addr >= 0x2000 && addr <= 0x3FFF)
        ppu.CPUWrite(addr & 0x0007, data);

    else
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

    else
        throw std::out_of_range("Specified address is outside the bus' range.");

    return data;
}

void Bus::InsertCartridge(const std::shared_ptr<Cartridge> &cartridge) {
    m_Cartridge = cartridge;
    ppu.ConnectCatridge(cartridge);
}

void Bus::Clock() {
    ppu.Clock();
    if (m_ClockCounter % 3 == 0) cpu.clock();
    m_ClockCounter++;
}

void Bus::Reset() {
    cpu.reset();
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