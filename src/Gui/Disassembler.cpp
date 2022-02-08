#include "Disassembler.hpp"

#include <SDL_events.h>

#include <algorithm>
#include <sstream>
#include <type_traits>

#include "../Common/Costants.hpp"
#include "DrawableTexture.hpp"
#include "Label.hpp"
#include "Renderer.hpp"

namespace Emulation {

template <typename T>
requires std::integral<T>
static std::string ToHex(T n) {
    // https://stackoverflow.com/a/33447587/13166735

    constexpr auto stringLength = sizeof(T) << 1;
    constexpr char digits[] = "0123456789ABCDEF";
    std::string result(stringLength, '0');
    for (size_t i = 0, j = (stringLength - 1) * 4; i < stringLength;
         ++i, j -= 4)
        result[i] = digits[(n >> j) & 0x0f];
    return result;
};

Disassembler::Disassembler(Bus& bus, GUI::Renderer& renderer, AssetManager& am,
                           EventHandler& em)
    : m_Bus(bus),
      m_Renderer(renderer),
      m_Font(am.LoadFont(DEBUG_FONT.path, DEBUG_FONT.name.data(),
                         DEBUG_FONT.fontSize)),
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
      }) {
    em.AddListener(Listener{
        .Type = SDL_KEYDOWN,
        .Do =
            [&](SDL_Event& e) {
                if (e.key.keysym.scancode == SDL_SCANCODE_P)
                    (++m_Palettes.Selected.ID) &= 0x07;
            },
    });
    Disassemble(0x0000, 0xFFFF);
}

void Disassembler::Render() {
    m_NextPosition = {NES_EMULATOR_SCREEN_SIZE.x + leftPadding, 10};
    CPUStatus();
    CPURegisters();
    MemoryDisassembly();
    PPUPalettes();
    PPUSprites();
}

void Disassembler::CPUStatus() {
    m_Renderer.DrawText(m_CPURegisters.Status.StatusLabel, m_NextPosition);
    m_NextPosition.x += DEBUG_FONT.charWidth * 9;

#define DISPLAY_STATUS(i, name)                                             \
    {                                                                       \
        m_CPURegisters.Status.Bits[0].SetColor(                             \
            m_Bus.m_CPU.stat.name ? Color::green : Color::red);             \
        m_Renderer.DrawText(m_CPURegisters.Status.Bits[i], m_NextPosition); \
        m_NextPosition.x += 2 * DEBUG_FONT.charWidth;                       \
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
        m_NextPosition.x += 3 * DEBUG_FONT.charWidth;                          \
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

void Disassembler::Disassemble(u16 start, u16 end) {
    u16 lineAddr = 0;
    u8 high = 0, low = 0, value = 0;
    u32 addr = start;

    while (addr <= (u32)end) {
        lineAddr = addr;
        std::stringstream ss;
        ss << "0x" << ToHex(lineAddr) << " |  ";

        u8 opcode = m_Bus.CPURead(addr++, true);
        ss << m_Bus.m_CPU.m_Lookup[opcode].name << " - ";

        const auto addrMode = m_Bus.m_CPU.m_Lookup[opcode].addrmode;

        if (addrMode == &CPU::IMP) {
            ss << "[IMP]";
        } else {
            ss << "$";
            if (addrMode == &CPU::IMM) {
                value = m_Bus.CPURead(addr++, true);
                ss << ToHex(value) << " [IMM]";
            } else if (addrMode == &CPU::ZP0) {
                low = m_Bus.CPURead(addr++, true);
                high = 0x00;
                ss << ToHex(low) << " [ZP0]";
            } else if (addrMode == &CPU::ZPX) {
                low = m_Bus.CPURead(addr++, true);
                high = 0x00;
                ss << ToHex(low) << "+ X [ZPX]";
            } else if (addrMode == &CPU::ZPY) {
                low = m_Bus.CPURead(addr++, true);
                high = 0x00;
                ss << ToHex(low) << "+ Y [ZPY]";
            } else if (addrMode == &CPU::IZX) {
                low = m_Bus.CPURead(addr++, true);
                high = 0x00;
                ss << ToHex(low) << "+ X [IZX]";
            } else if (addrMode == &CPU::IZY) {
                low = m_Bus.CPURead(addr++, true);
                high = 0x00;
                ss << ToHex(low) << "+ Y [IZY]";
            } else if (addrMode == &CPU::ABS) {
                low = m_Bus.CPURead(addr++, true);
                high = m_Bus.CPURead(addr++, true);
                ss << ToHex((u16)(high << 8) | low) << " [ABS]";
            } else if (addrMode == &CPU::ABX) {
                low = m_Bus.CPURead(addr++, true);
                high = m_Bus.CPURead(addr++, true);
                ss << ToHex((u16)(high << 8) | low) << "+ X [ABX]";
            } else if (addrMode == &CPU::ABY) {
                low = m_Bus.CPURead(addr++, true);
                high = m_Bus.CPURead(addr++, true);
                ss << ToHex((u16)(high << 8) | low) << "+ Y [ABY]";
            } else if (addrMode == &CPU::IND) {
                low = m_Bus.CPURead(addr++, true);
                high = m_Bus.CPURead(addr++, true);
                ss << "(" << ToHex((u16)(high << 8) | low) << ") [IND]";
            } else if (addrMode == &CPU::REL) {
                value = m_Bus.CPURead(addr++, true);
                ss << ToHex(value) << " ($" << ToHex(addr + (i8)value)
                   << ") [REL]";
            }
        }

        m_DisassembledCode.insert_or_assign(lineAddr,
                                            GUI::Label(ss.str(), m_Font));
    }
}

void Disassembler::NextLine() {
    m_NextPosition = {NES_EMULATOR_SCREEN_SIZE.x + leftPadding,
                      m_NextPosition.y + DEBUG_FONT.charHeight};
}

void Disassembler::MemoryDisassembly() {
    NextLine();
    auto it = m_DisassembledCode.find(m_Bus.m_CPU.pc);

    for (i32 i = 0; i <= 10; i++) {
        if (--it == m_DisassembledCode.end()) {
            break;
        }
    }

    for (i32 i = 0; i < 10; i++) {
        if (++it != m_DisassembledCode.end()) {
            m_Renderer.DrawText(it->second, m_NextPosition);
            NextLine();
        }
    }

    it = m_DisassembledCode.find(m_Bus.m_CPU.pc);

    if (it != m_DisassembledCode.end()) {
        it->second.SetColor(Color::blue);
        m_Renderer.DrawText(it->second, m_NextPosition);
        it->second.SetColor(Color::white);
        NextLine();

        for (i32 i = 0; i < 10; i++) {
            if (++it != m_DisassembledCode.end()) {
                m_Renderer.DrawText(it->second, m_NextPosition);
                NextLine();
            }
        }
    }
}

void Disassembler::PPUPalettes() {
    NextLine();

    for (i32 paletteID = 0; paletteID < 8; paletteID++) {
        if (paletteID == m_Palettes.Selected.ID) {
            m_Renderer.DrawTexture(m_Palettes.Selected.Selected,
                                   {m_NextPosition.x, m_NextPosition.y - 4});
        }

        for (i32 pixelValue = 0; pixelValue < 4; pixelValue++) {
            m_Palettes.Palettes[paletteID].Colors[pixelValue].Tint =
                m_Bus.m_PPU.GetColorFromPalette(paletteID, pixelValue);

            m_Renderer.DrawTexture(
                m_Palettes.Palettes[paletteID].Colors[pixelValue],
                m_NextPosition);
            m_NextPosition.x += paletteSize;
        }
        m_NextPosition.x += 10;
    }
    NextLine();
}

void Disassembler::PPUSprites() {
    NextLine();

    m_Bus.m_PPU.UpdatePatternTables(m_Palettes.Selected.ID);

    m_Renderer.DrawTexture(m_Bus.m_PPU.m_PatternTableSprites[0], m_NextPosition,
                           {2, 2});
    m_NextPosition.x += 128 * 2 + 15;
    m_Renderer.DrawTexture(m_Bus.m_PPU.m_PatternTableSprites[1], m_NextPosition,
                           {2, 2});
    m_NextPosition.y += 128 * 2;
    NextLine();
}

}  // namespace Emulation