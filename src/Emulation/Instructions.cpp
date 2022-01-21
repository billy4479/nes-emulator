#include "CPU.hpp"

namespace Emulation {

// Addition with Carry
u8 CPU::ADC() {
    Fetch();

    // Sum data (16-bit due to the carry)
    u16 tmp = (u16)a + (u16)m_Fetched + (u16)stat.C;

    stat.C = tmp > 255;
    UPDATE_NZ((tmp & 0x00FF));
    stat.V =
        (~((u16)a ^ (u16)m_Fetched) & ((u16)a ^ tmp) &
         0x0080);  // See
                   // https://github.com/OneLoneCoder/olcNES/blob/master/Part%232%20-%20CPU/olc6502.cpp#L659
    a = tmp & 0x00FF;
    return 1;
}

// Logical AND
u8 CPU::AND() {
    Fetch();
    a &= m_Fetched;
    UPDATE_NZ(a);
    return 1;
}

// Arithmetic Shift Left
u8 CPU::ASL() {
    Fetch();
    u16 tmp = (u16)m_Fetched << 1;
    stat.C = tmp & 0xFF00;
    UPDATE_NZ((tmp & 0x00FF));

    if (m_Lookup[m_Opcode].addrmode == &CPU::IMP)
        a = tmp & 0x00FF;
    else
        Write(m_AddrAbs, tmp & 0x00FF);
    return 0;
}

// Branch if the carry bit is clear
u8 CPU::BCC() {
    if (stat.C == 0) {
        m_CyclesLeft++;
        m_AddrAbs = pc + m_AddrRel;

        if ((m_AddrAbs & 0xFF00) != (pc & 0xFF00)) m_CyclesLeft++;

        pc = m_AddrAbs;
    }
    return 0;
}

// Branch if the carry bit is set
u8 CPU::BCS() {
    if (stat.C == 1) {
        m_CyclesLeft++;
        m_AddrAbs = pc + m_AddrRel;

        if ((m_AddrAbs & 0xFF00) != (pc & 0xFF00)) m_CyclesLeft++;

        pc = m_AddrAbs;
    }
    return 0;
}

// Branch if equal
u8 CPU::BEQ() {
    if (stat.Z == 1) {
        m_CyclesLeft++;
        m_AddrAbs = pc + m_AddrRel;

        if ((m_AddrAbs & 0xFF00) != (pc & 0xFF00)) m_CyclesLeft++;

        pc = m_AddrAbs;
    }
    return 0;
}

// Bit Test
u8 CPU::BIT() {
    Fetch();
    u8 tmp = a & m_Fetched;

    stat.Z = tmp == 0x00;
    stat.N = tmp & (1 << 7);
    stat.V = tmp & (1 << 6);

    return 0;
}

// Branch if Minus
u8 CPU::BMI() {
    if (stat.N == 1) {
        m_CyclesLeft++;
        m_AddrAbs = pc + m_AddrRel;

        if ((m_AddrAbs & 0xFF00) != (pc & 0xFF00)) m_CyclesLeft++;

        pc = m_AddrAbs;
    }
    return 0;
}

// Branch if Not Equal
u8 CPU::BNE() {
    if (stat.Z == 0) {
        m_CyclesLeft++;
        m_AddrAbs = pc + m_AddrRel;

        if ((m_AddrAbs & 0xFF00) != (pc & 0xFF00)) m_CyclesLeft++;

        pc = m_AddrAbs;
    }
    return 0;
}

// Branch if Positive
u8 CPU::BPL() {
    if (stat.N == 0) {
        m_CyclesLeft++;
        m_AddrAbs = pc + m_AddrRel;

        if ((m_AddrAbs & 0xFF00) != (pc & 0xFF00)) m_CyclesLeft++;

        pc = m_AddrAbs;
    }
    return 0;
}

// Force Interrupt
u8 CPU::BRK() {
    pc++;

    stat.I = 1;
    Write(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp--;
    Write(0x0100 + sp, pc & 0x00FF);
    sp--;

    stat.B = 1;
    Write(0x0100 + sp, stat);
    sp--;
    stat.B = 0;

    pc = (u16)Read(0xFFFE) | ((u16)Read(0xFFFE) << 8);
    return 0;
}

// Branch if Overflow Clear
u8 CPU::BVC() {
    if (stat.V == 0) {
        m_CyclesLeft++;
        m_AddrAbs = pc + m_AddrRel;

        if ((m_AddrAbs & 0xFF00) != (pc & 0xFF00)) m_CyclesLeft++;

        pc = m_AddrAbs;
    }
    return 0;
}

// Branch if Overflow Set
u8 CPU::BVS() {
    if (stat.V == 1) {
        m_CyclesLeft++;
        m_AddrAbs = pc + m_AddrRel;

        if ((m_AddrAbs & 0xFF00) != (pc & 0xFF00)) m_CyclesLeft++;

        pc = m_AddrAbs;
    }
    return 0;
}

// Clear Carry Flag
u8 CPU::CLC() {
    stat.C = 0;
    return 0;
}

// Clear Decimal Mode
u8 CPU::CLD() {
    stat.D = 0;
    return 0;
}

// Clear Interrupt Disable
u8 CPU::CLI() {
    stat.I = 0;
    return 0;
}

// Clear Overflow Flag
u8 CPU::CLV() {
    stat.V = 0;
    return 0;
}

// Compare accumulator
u8 CPU::CMP() {
    Fetch();
    u16 tmp = (u16)a - (u16)m_Fetched;
    stat.C = a >= m_Fetched;
    UPDATE_NZ((tmp & 0x00FF));
    return 1;
}

// Compare X Register
u8 CPU::CPX() {
    Fetch();
    u16 tmp = (u16)x - (u16)m_Fetched;
    stat.C = x >= m_Fetched;
    UPDATE_NZ((tmp & 0x00FF));
    return 1;
}

// Compare Y Register
u8 CPU::CPY() {
    Fetch();
    u16 tmp = (u16)y - (u16)m_Fetched;
    stat.C = y >= m_Fetched;
    UPDATE_NZ((tmp & 0x00FF));
    return 1;
}

// Decrement Memory
u8 CPU::DEC() {
    Fetch();
    u8 tmp = m_Fetched - 1;
    Write(m_AddrAbs, tmp);
    UPDATE_NZ(tmp);
    return 0;
}

// Decremt X Register
u8 CPU::DEX() {
    x--;
    UPDATE_NZ(x);
    return 0;
}

// Decremt Y Register
u8 CPU::DEY() {
    y--;
    UPDATE_NZ(y);
    return 0;
}

// Bitwise XOR
u8 CPU::EOR() {
    Fetch();
    a ^= m_Fetched;
    UPDATE_NZ(a);
    return 1;
}

// Increment Memory
u8 CPU::INC() {
    Fetch();
    u16 tmp = ++m_Fetched;
    Write(m_AddrAbs, tmp & 0x00FF);
    UPDATE_NZ((tmp & 0x00FF));
    return 0;
}

// Increment X Register
u8 CPU::INX() {
    x++;
    UPDATE_NZ(x);
    return 0;
}

// Increment Y Register
u8 CPU::INY() {
    y++;
    UPDATE_NZ(x);
    return 0;
}

// Jump to Location
u8 CPU::JMP() {
    pc = m_AddrAbs;
    return 0;
}

// Jump to sub-routine
u8 CPU::JSR() {
    pc--;

    Write(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp--;
    Write(0x0100 + sp, pc & 0x00FF);
    sp--;

    pc = m_AddrAbs;
    return 0;
}

// Load accumulator
u8 CPU::LDA() {
    Fetch();
    a = m_Fetched;
    UPDATE_NZ(a);
    return 1;
}

// Load X register
u8 CPU::LDX() {
    Fetch();
    x = m_Fetched;
    UPDATE_NZ(x);
    return 1;
}

// Load Y register
u8 CPU::LDY() {
    Fetch();
    y = m_Fetched;
    UPDATE_NZ(y);
    return 1;
}

// Logical Shift Right
u8 CPU::LSR() {
    Fetch();
    stat.C = m_Fetched & 0x0001;
    u8 tmp = m_Fetched >> 1;
    UPDATE_NZ(tmp);
    if (m_Lookup[m_Opcode].addrmode == &CPU::IMP)
        a = tmp;
    else
        Write(m_AddrAbs, tmp);
    return 0;
}

u8 CPU::NOP() {
    // Yay, illegal opcodes!
    // https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes

    switch (m_Opcode) {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
            break;
    }
    return 0;
}

// Bitwise Logic OR
u8 CPU::ORA() {
    Fetch();
    a |= m_Fetched;
    UPDATE_NZ(a);
    return 1;
}

// Push A to stack
u8 CPU::PHA() {
    Write(0x100 + sp, a);
    sp--;
    return 0;
}

// Push Status Register
u8 CPU::PHP() {
    auto tmp = stat;
    tmp.B = 1;
    tmp.U = 1;
    // Note: break flag is set to 1 before push
    Write(0x0100 + sp, tmp);

    stat.B = 0;
    stat.U = 0;
    sp--;
    return 0;
}

// Pop A from stack
u8 CPU::PLA() {
    a = Read(0x100 + ++sp);
    stat.Z = a == 0x00;
    stat.N = a & 0x80;
    return 0;
}

// Pop Status Register
u8 CPU::PLP() {
    sp++;
    stat = Read(0x0100 + sp);
    stat.U = 1;
    return 0;
}

// Rotate Left
u8 CPU::ROL() {
    Fetch();
    u16 tmp = (m_Fetched << 1) | stat.C;
    UPDATE_NZ(tmp);
    stat.C = tmp & 0xFF00;

    if (m_Lookup[m_Opcode].addrmode == &CPU::IMP)
        a = tmp & 0x00FF;
    else
        Write(m_AddrAbs, tmp & 0x00FF);

    return 0;
}

// Rotate Right
u8 CPU::ROR() {
    Fetch();
    u16 tmp = (u16)(stat.C << 7) | (m_Fetched >> 1);
    UPDATE_NZ(tmp);
    stat.C = m_Fetched & 0x01;

    if (m_Lookup[m_Opcode].addrmode == &CPU::IMP)
        a = tmp & 0x00FF;
    else
        Write(m_AddrAbs, tmp & 0x00FF);

    return 0;
}

// Return from interrupt
u8 CPU::RTI() {
    stat = Read(0x0100 + ++sp);
    stat.B = 0;
    stat.U = 0;

    pc = (u16)Read(0x0100 + ++sp);
    pc = (u16)Read(0x0100 + ++sp) << 8;
    return 0;
}

// Return from Subroutine
u8 CPU::RTS() {
    sp++;
    pc = (u16)Read(0x0100 + sp);
    sp++;
    pc |= (u16)Read(0x0100 + sp) << 8;

    pc++;
    return 0;
}

// Subtraction with Carry
u8 CPU::SBC() {
    // This is about the same as addition
    Fetch();

    u16 value = ((u16)m_Fetched) ^ 0x00FF;

    u16 tmp = (u16)a + value + (u16)stat.C;

    stat.C = tmp > 255;
    stat.Z = (tmp & 0x00FF) == 0;
    stat.N = tmp & 0x80;
    stat.V = ((u16)a ^ value) & ((u16)a ^ (u16)tmp) & 0x0080;
    a = tmp & 0x00FF;
    return 1;
}

// Set carry Flag
u8 CPU::SEC() {
    stat.C = 1;
    return 0;
}

// Set Decimal Mode
u8 CPU::SED() {
    stat.D = 1;
    return 0;
}

// Set Interrupt Disable
u8 CPU::SEI() {
    stat.I = 1;
    return 0;
}

// Store Accumulator
u8 CPU::STA() {
    Write(m_AddrAbs, a);
    return 0;
}

// Store X
u8 CPU::STX() {
    Write(m_AddrAbs, x);
    return 0;
}

// Store Y
u8 CPU::STY() {
    Write(m_AddrAbs, y);
    return 0;
}

// Transfer Accumulator to X
u8 CPU::TAX() {
    x = a;
    UPDATE_NZ(x);
    return 0;
}

// Transfer Accumulator to Y
u8 CPU::TAY() {
    y = a;
    UPDATE_NZ(y);
    return 0;
}

// Transfer Stack pointer to X
u8 CPU::TSX() {
    x = sp;
    UPDATE_NZ(x);
    return 0;
}

// Transfer X to Accumulator
u8 CPU::TXA() {
    a = x;
    UPDATE_NZ(a);
    return 0;
}

// Transfer X to Stack pointer
u8 CPU::TXS() {
    sp = x;
    return 0;
}

// Transfer Y to Accumulator
u8 CPU::TYA() {
    a = y;
    UPDATE_NZ(a);
    return 0;
}

// Illegal opcodes
u8 CPU::XXX() { return 0; }

}  // namespace Emulation