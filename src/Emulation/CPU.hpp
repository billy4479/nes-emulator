#pragma once
#include "../Common/Common.hpp"
#include <vector>

namespace Emulation {

    // Foreward declaration to avoid circular inclusion
    class Bus;

    class CPU {
      public:
        CPU();
        ~CPU();

        void reset(); // Reset CPU state
        void irq();   // Interrupt Request
        void nmi();   // Non-Maskable Interrupt Request
        void clock(); // Execute one clock cycle

        void ConnectToBus(Bus *b) { bus = b; }

        /*  *** OPCodes *** */
#pragma region OPCodes
        u8 ADC();
        u8 AND();
        u8 ASL();
        u8 BCC();
        u8 BCS();
        u8 BEQ();
        u8 BIT();
        u8 BMI();
        u8 BNE();
        u8 BPL();
        u8 BRK();
        u8 BVC();
        u8 BVS();
        u8 CLC();
        u8 CLD();
        u8 CLI();
        u8 CLV();
        u8 CMP();
        u8 CPX();
        u8 CPY();
        u8 DEC();
        u8 DEX();
        u8 DEY();
        u8 EOR();
        u8 INC();
        u8 INX();
        u8 INY();
        u8 JMP();
        u8 JSR();
        u8 LDA();
        u8 LDX();
        u8 LDY();
        u8 LSR();
        u8 NOP();
        u8 ORA();
        u8 PHA();
        u8 PHP();
        u8 PLA();
        u8 PLP();
        u8 ROL();
        u8 ROR();
        u8 RTI();
        u8 RTS();
        u8 SBC();
        u8 SEC();
        u8 SED();
        u8 SEI();
        u8 STA();
        u8 STX();
        u8 STY();
        u8 TAX();
        u8 TAY();
        u8 TSX();
        u8 TXA();
        u8 TXS();
        u8 TYA();

        u8 XXX();
#pragma endregion OPCodes

/*  *** Addressing Modes *** */
#pragma region AddrModes
        u8 IMP();
        u8 IMM();
        u8 ZP0();
        u8 ZPX();
        u8 ZPY();
        u8 REL();
        u8 ABS();
        u8 ABX();
        u8 ABY();
        u8 IND();
        u8 IZX();
        u8 IZY();
#pragma endregion AddrModes

        u8 fetch();        // Fetch data at the Program Counter
        u8 fetched = 0x00; // Data received from fetch()

        u16 addrAbs = 0x0000;
        u16 addrRel = 0x0000;
        u8 opcode = 0x00;
        u8 cycles = 0; // Cycles left to finish the instruction

      private:
        /*  *** Status Flags *** */
        struct Status {
            u8 C : 1; // Carry bit
            u8 Z : 1; // Zero
            u8 I : 1; // Disable Interrupts
            u8 D : 1; // Decimal Mode
            u8 B : 1; // Break
            u8 U : 1; // Unused
            u8 V : 1; // Overflow
            u8 N : 1; // Negative

            Status() = default;
            Status(u8 c) { *this = c; }
            operator u8() { return (u8) * this; }
        };

        /*  *** Registers *** */
        u8 a = 0x00;        // Accumulator
        u8 x = 0x00;        // X
        u8 y = 0x00;        // Y
        u8 sp = 0x00;       // Stack Pointer
        u16 pc = 0x0000;    // Program Counter
        Status stat = 0x00; // Status flags

        Bus *bus = nullptr;

        /*  *** Read & Write from the bus *** */
        void write(u16 addr, u8 data);
        u8 read(u16 addr);

        struct Instruction {
            char name[4]; // 3 + null byte
            u8 (CPU::*operation)(void) = nullptr;
            u8 (CPU::*addrmode)(void) = nullptr;
            u8 cycles = 0;
        };

        std::vector<Instruction> lookup;

#define UPDATE_Z(R) stat.Z = (R) == 0x00
#define UPDATE_N(R) stat.N = (R)&0x80
#define UPDATE_NZ(R)                                                           \
    UPDATE_Z(R);                                                               \
    UPDATE_N(R);
    };
} // namespace Emulation