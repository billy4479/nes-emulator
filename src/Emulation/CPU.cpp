#include "CPU.hpp"
#include "Bus.hpp"

namespace Emulation {

    CPU::CPU() {
        using a = CPU;
        constexpr Instruction inst[] = {
            {"BRK", &a::BRK, &a::IMM, 7}, {"ORA", &a::ORA, &a::IZX, 6},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"???", &a::NOP, &a::IMP, 3}, {"ORA", &a::ORA, &a::ZP0, 3},
            {"ASL", &a::ASL, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
            {"PHP", &a::PHP, &a::IMP, 3}, {"ORA", &a::ORA, &a::IMM, 2},
            {"ASL", &a::ASL, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
            {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ABS, 4},
            {"ASL", &a::ASL, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"BPL", &a::BPL, &a::REL, 2}, {"ORA", &a::ORA, &a::IZY, 5},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ZPX, 4},
            {"ASL", &a::ASL, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"CLC", &a::CLC, &a::IMP, 2}, {"ORA", &a::ORA, &a::ABY, 4},
            {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
            {"???", &a::NOP, &a::IMP, 4}, {"ORA", &a::ORA, &a::ABX, 4},
            {"ASL", &a::ASL, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
            {"JSR", &a::JSR, &a::ABS, 6}, {"AND", &a::AND, &a::IZX, 6},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"BIT", &a::BIT, &a::ZP0, 3}, {"AND", &a::AND, &a::ZP0, 3},
            {"ROL", &a::ROL, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
            {"PLP", &a::PLP, &a::IMP, 4}, {"AND", &a::AND, &a::IMM, 2},
            {"ROL", &a::ROL, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
            {"BIT", &a::BIT, &a::ABS, 4}, {"AND", &a::AND, &a::ABS, 4},
            {"ROL", &a::ROL, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"BMI", &a::BMI, &a::REL, 2}, {"AND", &a::AND, &a::IZY, 5},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"???", &a::NOP, &a::IMP, 4}, {"AND", &a::AND, &a::ZPX, 4},
            {"ROL", &a::ROL, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"SEC", &a::SEC, &a::IMP, 2}, {"AND", &a::AND, &a::ABY, 4},
            {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
            {"???", &a::NOP, &a::IMP, 4}, {"AND", &a::AND, &a::ABX, 4},
            {"ROL", &a::ROL, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
            {"RTI", &a::RTI, &a::IMP, 6}, {"EOR", &a::EOR, &a::IZX, 6},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"???", &a::NOP, &a::IMP, 3}, {"EOR", &a::EOR, &a::ZP0, 3},
            {"LSR", &a::LSR, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
            {"PHA", &a::PHA, &a::IMP, 3}, {"EOR", &a::EOR, &a::IMM, 2},
            {"LSR", &a::LSR, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
            {"JMP", &a::JMP, &a::ABS, 3}, {"EOR", &a::EOR, &a::ABS, 4},
            {"LSR", &a::LSR, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"BVC", &a::BVC, &a::REL, 2}, {"EOR", &a::EOR, &a::IZY, 5},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"???", &a::NOP, &a::IMP, 4}, {"EOR", &a::EOR, &a::ZPX, 4},
            {"LSR", &a::LSR, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"CLI", &a::CLI, &a::IMP, 2}, {"EOR", &a::EOR, &a::ABY, 4},
            {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
            {"???", &a::NOP, &a::IMP, 4}, {"EOR", &a::EOR, &a::ABX, 4},
            {"LSR", &a::LSR, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
            {"RTS", &a::RTS, &a::IMP, 6}, {"ADC", &a::ADC, &a::IZX, 6},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"???", &a::NOP, &a::IMP, 3}, {"ADC", &a::ADC, &a::ZP0, 3},
            {"ROR", &a::ROR, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
            {"PLA", &a::PLA, &a::IMP, 4}, {"ADC", &a::ADC, &a::IMM, 2},
            {"ROR", &a::ROR, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
            {"JMP", &a::JMP, &a::IND, 5}, {"ADC", &a::ADC, &a::ABS, 4},
            {"ROR", &a::ROR, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"BVS", &a::BVS, &a::REL, 2}, {"ADC", &a::ADC, &a::IZY, 5},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"???", &a::NOP, &a::IMP, 4}, {"ADC", &a::ADC, &a::ZPX, 4},
            {"ROR", &a::ROR, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"SEI", &a::SEI, &a::IMP, 2}, {"ADC", &a::ADC, &a::ABY, 4},
            {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
            {"???", &a::NOP, &a::IMP, 4}, {"ADC", &a::ADC, &a::ABX, 4},
            {"ROR", &a::ROR, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
            {"???", &a::NOP, &a::IMP, 2}, {"STA", &a::STA, &a::IZX, 6},
            {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 6},
            {"STY", &a::STY, &a::ZP0, 3}, {"STA", &a::STA, &a::ZP0, 3},
            {"STX", &a::STX, &a::ZP0, 3}, {"???", &a::XXX, &a::IMP, 3},
            {"DEY", &a::DEY, &a::IMP, 2}, {"???", &a::NOP, &a::IMP, 2},
            {"TXA", &a::TXA, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
            {"STY", &a::STY, &a::ABS, 4}, {"STA", &a::STA, &a::ABS, 4},
            {"STX", &a::STX, &a::ABS, 4}, {"???", &a::XXX, &a::IMP, 4},
            {"BCC", &a::BCC, &a::REL, 2}, {"STA", &a::STA, &a::IZY, 6},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 6},
            {"STY", &a::STY, &a::ZPX, 4}, {"STA", &a::STA, &a::ZPX, 4},
            {"STX", &a::STX, &a::ZPY, 4}, {"???", &a::XXX, &a::IMP, 4},
            {"TYA", &a::TYA, &a::IMP, 2}, {"STA", &a::STA, &a::ABY, 5},
            {"TXS", &a::TXS, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 5},
            {"???", &a::NOP, &a::IMP, 5}, {"STA", &a::STA, &a::ABX, 5},
            {"???", &a::XXX, &a::IMP, 5}, {"???", &a::XXX, &a::IMP, 5},
            {"LDY", &a::LDY, &a::IMM, 2}, {"LDA", &a::LDA, &a::IZX, 6},
            {"LDX", &a::LDX, &a::IMM, 2}, {"???", &a::XXX, &a::IMP, 6},
            {"LDY", &a::LDY, &a::ZP0, 3}, {"LDA", &a::LDA, &a::ZP0, 3},
            {"LDX", &a::LDX, &a::ZP0, 3}, {"???", &a::XXX, &a::IMP, 3},
            {"TAY", &a::TAY, &a::IMP, 2}, {"LDA", &a::LDA, &a::IMM, 2},
            {"TAX", &a::TAX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
            {"LDY", &a::LDY, &a::ABS, 4}, {"LDA", &a::LDA, &a::ABS, 4},
            {"LDX", &a::LDX, &a::ABS, 4}, {"???", &a::XXX, &a::IMP, 4},
            {"BCS", &a::BCS, &a::REL, 2}, {"LDA", &a::LDA, &a::IZY, 5},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 5},
            {"LDY", &a::LDY, &a::ZPX, 4}, {"LDA", &a::LDA, &a::ZPX, 4},
            {"LDX", &a::LDX, &a::ZPY, 4}, {"???", &a::XXX, &a::IMP, 4},
            {"CLV", &a::CLV, &a::IMP, 2}, {"LDA", &a::LDA, &a::ABY, 4},
            {"TSX", &a::TSX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 4},
            {"LDY", &a::LDY, &a::ABX, 4}, {"LDA", &a::LDA, &a::ABX, 4},
            {"LDX", &a::LDX, &a::ABY, 4}, {"???", &a::XXX, &a::IMP, 4},
            {"CPY", &a::CPY, &a::IMM, 2}, {"CMP", &a::CMP, &a::IZX, 6},
            {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"CPY", &a::CPY, &a::ZP0, 3}, {"CMP", &a::CMP, &a::ZP0, 3},
            {"DEC", &a::DEC, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
            {"INY", &a::INY, &a::IMP, 2}, {"CMP", &a::CMP, &a::IMM, 2},
            {"DEX", &a::DEX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 2},
            {"CPY", &a::CPY, &a::ABS, 4}, {"CMP", &a::CMP, &a::ABS, 4},
            {"DEC", &a::DEC, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"BNE", &a::BNE, &a::REL, 2}, {"CMP", &a::CMP, &a::IZY, 5},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"???", &a::NOP, &a::IMP, 4}, {"CMP", &a::CMP, &a::ZPX, 4},
            {"DEC", &a::DEC, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"CLD", &a::CLD, &a::IMP, 2}, {"CMP", &a::CMP, &a::ABY, 4},
            {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
            {"???", &a::NOP, &a::IMP, 4}, {"CMP", &a::CMP, &a::ABX, 4},
            {"DEC", &a::DEC, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
            {"CPX", &a::CPX, &a::IMM, 2}, {"SBC", &a::SBC, &a::IZX, 6},
            {"???", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"CPX", &a::CPX, &a::ZP0, 3}, {"SBC", &a::SBC, &a::ZP0, 3},
            {"INC", &a::INC, &a::ZP0, 5}, {"???", &a::XXX, &a::IMP, 5},
            {"INX", &a::INX, &a::IMP, 2}, {"SBC", &a::SBC, &a::IMM, 2},
            {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::SBC, &a::IMP, 2},
            {"CPX", &a::CPX, &a::ABS, 4}, {"SBC", &a::SBC, &a::ABS, 4},
            {"INC", &a::INC, &a::ABS, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"BEQ", &a::BEQ, &a::REL, 2}, {"SBC", &a::SBC, &a::IZY, 5},
            {"???", &a::XXX, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 8},
            {"???", &a::NOP, &a::IMP, 4}, {"SBC", &a::SBC, &a::ZPX, 4},
            {"INC", &a::INC, &a::ZPX, 6}, {"???", &a::XXX, &a::IMP, 6},
            {"SED", &a::SED, &a::IMP, 2}, {"SBC", &a::SBC, &a::ABY, 4},
            {"NOP", &a::NOP, &a::IMP, 2}, {"???", &a::XXX, &a::IMP, 7},
            {"???", &a::NOP, &a::IMP, 4}, {"SBC", &a::SBC, &a::ABX, 4},
            {"INC", &a::INC, &a::ABX, 7}, {"???", &a::XXX, &a::IMP, 7},
        };

        lookup = std::vector<Instruction>(inst, inst + sizeof(inst) /
                                                           sizeof(Instruction));
    }

    CPU::~CPU() = default;

    void CPU::write(u16 addr, u8 data) { bus->Write(addr, data); }
    u8 CPU::read(u16 addr) { return bus->Read(addr, false); }

    void CPU::clock() {
        if (cycles == 0) {
            opcode = read(pc++);

            cycles = lookup[opcode].cycles;

            u8 additionalCycle1 = (this->*lookup[opcode].addrmode)();

            u8 additionalCycle2 = (this->*lookup[opcode].operation)();

            cycles += (additionalCycle1 & additionalCycle2);
        }

        cycles--;
    }

    u8 CPU::fetch() {
        if (lookup[opcode].addrmode != &CPU::IMP)
            fetched = read(addrAbs);

        return addrAbs;
    }

    void CPU::reset() {
        a = x = y = 0;
        sp = 0xFD;
        stat = 0x00;

        addrAbs = 0xFFFC;
        u16 lo = read(addrAbs);
        u16 hi = read(addrAbs + 1);
        pc = (hi << 8) | lo;

        addrRel = 0x0000;
        addrAbs = 0x0000;
        fetched = 0x00;

        cycles = 8;
    }

    void CPU::irq() {
        if (!stat.I) {
            // Push some registers onto the stack
            write(0x100 + sp, (pc >> 8) & 0x00FF);
            sp--;
            write(0x100 + sp, pc & 0x00FF);
            sp--;

            stat.B = 0;
            stat.U = 1;
            stat.I = 1;
            write(0x100 + sp, stat);
            sp--;

            addrAbs = 0xFFFE;
            u16 lo = read(addrAbs);
            u16 hi = read(addrAbs + 1);
            pc = (hi << 8) | lo;

            cycles = 7;
        }
    }

    void CPU::nmi() {
        // Push some registers onto the stack
        write(0x100 + sp, (pc >> 8) & 0x00FF);
        sp--;
        write(0x100 + sp, pc & 0x00FF);
        sp--;

        stat.B = 0;
        stat.U = 1;
        stat.I = 1;
        write(0x100 + sp, stat);
        sp--;

        addrAbs = 0xFFFE;
        u16 lo = read(addrAbs);
        u16 hi = read(addrAbs + 1);
        pc = (hi << 8) | lo;

        cycles = 7;
    }

} // namespace Emulation