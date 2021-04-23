#include "CPU.hpp"

namespace Emulation {

    // There's no data but may operate with the accumulator
    u8 CPU::IMP() {
        a = fetched;
        return 0;
    }

    // The data will be supplied in the next byte
    u8 CPU::IMM() {
        addrAbs = pc++;
        return 0;
    }

    // Read at page 0 the last byte at PC
    u8 CPU::ZP0() {
        addrAbs = read(pc++);
        addrAbs &= 0x00FF;
        return 0;
    }

    // Read at page 0 the last byte at PC + X
    u8 CPU::ZPX() {
        addrAbs = read((pc++) + x);
        addrAbs &= 0x00FF;
        return 0;
    }

    // Read at page 0 the last byte at PC + Y
    u8 CPU::ZPY() {
        addrAbs = read((pc++) + y);
        addrAbs &= 0x00FF;
        return 0;
    }

    // Absolute address
    u8 CPU::ABS() {
        u16 lo = read(pc++);
        u16 hi = read(pc++);

        addrAbs =
            (hi << 8) | lo; // Combining the 2 read bytes into a 16bit address
        return 0;
    }

    // Absolute address + X
    u8 CPU::ABX() {
        u16 lo = read(pc++);
        u16 hi = read(pc++);

        addrAbs =
            (hi << 8) | lo; // Combining the 2 read bytes into a 16bit address
        addrAbs += x;

        if ((addrAbs & 0xFF00) !=
            (hi << 8)) // If the page is changed we need an additional cycle
            return 1;
        return 0;
    }

    // Absolute address + X
    u8 CPU::ABY() {
        u16 lo = read(pc++);
        u16 hi = read(pc++);

        addrAbs =
            (hi << 8) | lo; // Combining the 2 read bytes into a 16bit address
        addrAbs += x;

        if ((addrAbs & 0xFF00) !=
            (hi << 8)) // If the page is changed we need an additional cycle
            return 1;
        return 0;
    }

    // Indirect Addressing (pointer)
    u8 CPU::IND() {
        u16 lo = read(pc++);
        u16 hi = read(pc++);

        u16 ptr = (hi << 8) | lo;

        if (lo == 0x00FF) // Simulate hardware bug that occurs when the pointer
                          // points to byte between 2 pages
            addrAbs = (read(ptr & 0xFF00) << 8 | read(ptr + 0));
        else
            addrAbs = (read(ptr + 1) << 8 | read(ptr)); // Read at address

        return 0;
    }

    // Indirect Addressing of the zeroth page with X offset (to the pointer)
    u8 CPU::IZX() {
        u16 ptr = read(pc++);

        u16 lo = read((u16)(ptr + (u16)x) & 0x00FF);
        u16 hi = read((u16)(ptr + 1 + (u16)x) & 0x00FF);

        addrAbs = (hi << 8) | lo;

        return 0;
    }

    // Indirect Addressing of the zeroth page with Y offset (to the address)
    u8 CPU::IZY() {
        u16 ptr = read(pc++);

        u16 lo = read(ptr & 0x00FF);
        u16 hi = read((ptr + 1) & 0x00FF);

        addrAbs = (hi << 8) | lo;
        addrAbs += y;

        // Check if we cross the pace boundary
        if ((addrAbs & 0xFF00) != (hi << 8))
            return 1;

        return 0;
    }

    u8 CPU::REL() {
        addrRel = read(pc++);

        // The read byte is signed: if it's negative we set the first byte to FF
        // to be recognized later
        if (addrRel & 0x80)
            addrRel |= 0xFF00;

        return 0;
    }

} // namespace Emulation