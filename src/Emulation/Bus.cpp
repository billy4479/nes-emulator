#include "Bus.hpp"
#include "CPU.hpp"
#include <assert.h>
#include <iomanip>
#include <sstream>

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

    std::string Bus::DumpRamAsHex(u16 start, u16 length) {
        std::stringstream ss;
        assert(start + length < ram.size());

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
            if (i % 16 == 0)
                ss << "\n" << hex_u16(i) << ":\t";
            ss << hex_u8(ram[i]) << " ";
        }

        ss << "\n";

        return ss.str();
    }

} // namespace Emulation