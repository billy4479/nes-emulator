#include "Bus.hpp"

namespace Emulation {
    Bus::Bus() {
        // Clear ram to 0;
        for (auto &i : ram)
            i = 0x00;

        cpu.ConnectToBus(this);
    }

    Bus::~Bus() {}

    void Bus::Write(u16 addr, u8 data) {
        if (addr >= 0x0000 && addr <= 0xFFFF)
            ram[addr] = data;
    }
    u8 Bus::Read(u16 addr, bool readOnly) {
        if (addr >= 0x0000 && addr <= 0xFFFF)
            return ram[addr];

        throw std::out_of_range("Specified address is outside the bus' range.");
    }

} // namespace Emulation