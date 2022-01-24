#include "Disassembler.hpp"

#include <sstream>
#include <type_traits>

#include "../Common/Costants.hpp"
#include "Label.hpp"
#include "Renderer.hpp"

namespace Emulation {

template <typename T>
requires std::integral<T>
static std::string ToHex(T n) {
    constexpr auto charsPerByte = sizeof(T) * 2;
    std::string s(charsPerByte, '0');
    for (int i = charsPerByte; i > 0; i--, n >>= 4)
        s[i] = "0123456789ABCDEF"[n & 0xF];
    return s;
};

Disassembler::Disassembler(const Bus& bus, GUI::Renderer& renderer,
                           AssetManager& am)
    : m_Bus(bus),
      m_Renderer(renderer),
      m_Font(am.GetFont("JetBrains Mono")),
      m_CPURegisters({
          .Status =
              {
                  .StatusLabel = GUI::Label("Status:", m_Font),
                  .Bits =
                      {
                          GUI::Label("C", m_Font),
                          GUI::Label("Z", m_Font),
                          GUI::Label("I", m_Font),
                          GUI::Label("D", m_Font),
                          GUI::Label("B", m_Font),
                          GUI::Label("-", m_Font),
                          GUI::Label("V", m_Font),
                          GUI::Label("N", m_Font),
                      },
              },
          .Registers =
              {
                  {
                      .Name = GUI::Label("A", m_Font),
                      .Value = GUI::Label("", m_Font),
                  },
                  {
                      .Name = GUI::Label("X", m_Font),
                      .Value = GUI::Label("", m_Font),
                  },
                  {
                      .Name = GUI::Label("Y", m_Font),
                      .Value = GUI::Label("", m_Font),
                  },
                  {
                      .Name = GUI::Label("SP", m_Font),
                      .Value = GUI::Label("", m_Font),
                  },
                  {
                      .Name = GUI::Label("PC", m_Font),
                      .Value = GUI::Label("", m_Font),
                  },
              },
      }) {}

void Disassembler::Render() {
    m_NextPosition = {NES_EMULATOR_SCREEN_SIZE.x + 15, 10};
    CPUStatus();
    CPURegisters();
    MemoryDisassembly();
    PPUPalettes();
    PPUSprites();
}

void Disassembler::CPUStatus() {
    m_Renderer.DrawText(m_CPURegisters.Status.StatusLabel, m_NextPosition);
    m_NextPosition.x += 135;

#define DISPLAY_STATUS(i, name)                                             \
    {                                                                       \
        m_CPURegisters.Status.Bits[0].SetColor(                             \
            m_Bus.m_CPU.stat.name ? Color::green : Color::red);             \
        m_Renderer.DrawText(m_CPURegisters.Status.Bits[i], m_NextPosition); \
        m_NextPosition.x += 2 * charWidth;                                  \
    }

    DISPLAY_STATUS(0, C);
    DISPLAY_STATUS(1, Z);
    DISPLAY_STATUS(2, I);
    DISPLAY_STATUS(3, D);
    DISPLAY_STATUS(4, B);
    DISPLAY_STATUS(5, U);
    DISPLAY_STATUS(6, V);
    DISPLAY_STATUS(7, N);

#undef DISPLAY_REGISTRY

    NextLine();
}

void Disassembler::CPURegisters() {
#define DISPLAY_REGISTER(i, name)                                              \
    {                                                                          \
        m_CPURegisters.Registers[i].Value.SetText(ToHex(m_Bus.m_CPU.name));    \
        m_Renderer.DrawText(m_CPURegisters.Registers[i].Name, m_NextPosition); \
        m_NextPosition.x += 3 * charWidth;                                     \
        m_Renderer.DrawText(m_CPURegisters.Registers[i].Value,                 \
                            m_NextPosition);                                   \
        NextLine();                                                            \
    }

    DISPLAY_REGISTER(0, a);
    DISPLAY_REGISTER(1, x);
    DISPLAY_REGISTER(2, y);
    DISPLAY_REGISTER(3, sp);
    DISPLAY_REGISTER(4, pc);

#undef DISPLAY_REGISTER
}

std::map<u16, std::string> Disassembler::Disassemble(u16 start, u16 end,
                                                     const Bus& bus) {
    std::map<u16, std::string> lines;
    u16 lineAddr = 0;
    u8 high = 0, low = 0, value = 0;

    for (auto addr = start; addr <= end; addr++) {
        lineAddr = addr;
        std::stringstream ss;
        ss << "0x" << ToHex(lineAddr) << " |  ";

        u8 opcode = bus.CPURead(addr++, true);
        ss << bus.m_CPU.m_Lookup[opcode].name << " - ";

        const auto addrMode = bus.m_CPU.m_Lookup[opcode].addrmode;

        if (addrMode == &CPU::IMP) {
            ss << "[IMP]";
        } else {
            ss << "$";
            if (addrMode == &CPU::IMM) {
                value = bus.CPURead(addr++, true);
                ss << ToHex(value) << " [IMM]";
            } else if (addrMode == &CPU::ZP0) {
                low = bus.CPURead(addr++, true);
                high = 0x00;
                ss << ToHex(low) << " [ZP0]";
            } else if (addrMode == &CPU::ZPX) {
                low = bus.CPURead(addr++, true);
                high = 0x00;
                ss << ToHex(low) << "+ X [ZPX]";
            } else if (addrMode == &CPU::ZPY) {
                low = bus.CPURead(addr++, true);
                high = 0x00;
                ss << ToHex(low) << "+ Y [ZPY]";
            } else if (addrMode == &CPU::IZX) {
                low = bus.CPURead(addr++, true);
                high = 0x00;
                ss << ToHex(low) << "+ X [IZX]";
            } else if (addrMode == &CPU::IZY) {
                low = bus.CPURead(addr++, true);
                high = 0x00;
                ss << ToHex(low) << "+ Y [IZY]";
            } else if (addrMode == &CPU::ABS) {
                low = bus.CPURead(addr++, true);
                high = bus.CPURead(addr++, true);
                ss << ToHex((u16)(high << 8) | low) << " [ABS]";
            } else if (addrMode == &CPU::ABX) {
                low = bus.CPURead(addr++, true);
                high = bus.CPURead(addr++, true);
                ss << ToHex((u16)(high << 8) | low) << "+ X [ABX]";
            } else if (addrMode == &CPU::ABY) {
                low = bus.CPURead(addr++, true);
                high = bus.CPURead(addr++, true);
                ss << ToHex((u16)(high << 8) | low) << "+ Y [ABY]";
            } else if (addrMode == &CPU::IND) {
                low = bus.CPURead(addr++, true);
                high = bus.CPURead(addr++, true);
                ss << "(" << ToHex((u16)(high << 8) | low) << ") [IND]";
            } else if (addrMode == &CPU::REL) {
                value = bus.CPURead(addr++, true);
                ss << ToHex(value) << " ($" << ToHex(addr + (i8)value)
                   << ") [REL]";
            }
        }

        lines[lineAddr] = ss.str();
    }

    return lines;
}

void Disassembler::NextLine() {
    m_NextPosition = {NES_EMULATOR_SCREEN_SIZE.x + leftPadding,
                      m_NextPosition.y + charHeight};
}

void Disassembler::MemoryDisassembly() {}

void Disassembler::PPUPalettes() {}

void Disassembler::PPUSprites() {}

}  // namespace Emulation