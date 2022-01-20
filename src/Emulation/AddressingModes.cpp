#include "CPU.hpp"

namespace Emulation {

// Addressing Mode: Implicit
// There's no data but may operate with the accumulator
u8 CPU::IMP() {
    a = fetched;
    return 0;
}

// Addressing Mode: Immediate
// The data will be supplied in the next byte
u8 CPU::IMM() {
    addrAbs = pc++;
    return 0;
}

// Addressing Mode: Zero Page
// Read at page 0 the last byte at PC
u8 CPU::ZP0() {
    addrAbs = Read(pc++);
    addrAbs &= 0x00FF;
    return 0;
}

// Addressing Mode: Zero Page, X
// Read at page 0 the last byte at PC + X
u8 CPU::ZPX() {
    addrAbs = Read((pc++) + x);
    addrAbs &= 0x00FF;
    return 0;
}

// Addressing Mode: Zero Page, Y
// Read at page 0 the last byte at PC + Y
u8 CPU::ZPY() {
    addrAbs = Read((pc++) + y);
    addrAbs &= 0x00FF;
    return 0;
}

// Addressing Mode: Absolute
u8 CPU::ABS() {
    u16 lo = Read(pc++);
    u16 hi = Read(pc++);

    addrAbs =
        (hi << 8) | lo;  // Combining the 2 read bytes into a 16bit address
    return 0;
}

// Addressing Mode: Absolute + X
u8 CPU::ABX() {
    u16 lo = Read(pc++);
    u16 hi = Read(pc++);

    addrAbs = (hi << 8) | lo;
    addrAbs += x;

    // If the page is changed we need an additional cycle
    if ((addrAbs & 0xFF00) != (hi << 8)) return 1;
    return 0;
}

// Addressing Mode: Absolute + Y
u8 CPU::ABY() {
    u16 lo = Read(pc++);
    u16 hi = Read(pc++);

    addrAbs = (hi << 8) | lo;
    addrAbs += x;

    // If the page is changed we need an additional cycle
    if ((addrAbs & 0xFF00) != (hi << 8)) return 1;
    return 0;
}

// Addressing Mode: Indirect
// Used only by JMP, works like a pointer
u8 CPU::IND() {
    u16 lo = Read(pc++);
    u16 hi = Read(pc++);

    u16 ptr = (hi << 8) | lo;

    // Simulate hardware bug that occurs when the pointer
    // points to byte between 2 pages.
    // The low byte is read correctly, the high one, instead of being read
    // from the next page is wrapped around and read from the first address
    // on the same page.
    if (lo == 0x00FF)
        addrAbs = (Read(ptr & 0xFF00) << 8 | Read(ptr));
    else
        // Behave normally
        addrAbs = (Read(ptr + 1) << 8 | Read(ptr));  // Read at address

    return 0;
}

// Addressing Mode: Indexed by X Indirect
// Read a byte at PC and sums it with the x register.
// The result is the address of the actual value on the zeroth page.
u8 CPU::IZX() {
    u8 ptr = Read(pc++);

    u16 lo = Read(((u16)ptr + (u16)x) & 0x00FF);
    u16 hi = Read(((u16)ptr + 1 + (u16)x) & 0x00FF);

    addrAbs = (hi << 8) | lo;

    return 0;
}

// Indirect Addressing of the zeroth page with Y offset (to the address)
u8 CPU::IZY() {
    u16 ptr = Read(pc++);

    u16 lo = Read(ptr & 0x00FF);
    u16 hi = Read((ptr + 1) & 0x00FF);

    addrAbs = (hi << 8) | lo;
    addrAbs += y;

    // Check if we cross the pace boundary
    if ((addrAbs & 0xFF00) != (hi << 8)) return 1;

    return 0;
}

// Addressing Mode: Relative
u8 CPU::REL() {
    addrRel = Read(pc++);

    // The read byte is signed: if it's negative we set the first byte to FF
    // to be recognized later
    if (addrRel & 0x80) addrRel |= 0xFF00;

    return 0;
}

}  // namespace Emulation