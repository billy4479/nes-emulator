#include "Disassembler.hpp"

#include <type_traits>

#include "../Common/Costants.hpp"
#include "Label.hpp"
#include "Renderer.hpp"

template <typename T>
requires std::integral<T>
static std::string to_hex(T n) {
    constexpr auto charsPerByte = sizeof(T) * 2;
    std::string s(charsPerByte, '0');
    for (int i = charsPerByte; i > 0; i--, n >>= 4)
        s[i] = "0123456789ABCDEF"[n & 0xF];
    return s;
};

Disassembler::Disassembler(const Emulation::Bus& bus, GUI::Renderer& renderer,
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
        m_NextPosition.x += 25;                                             \
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

    m_NextPosition = {NES_EMULATOR_SCREEN_SIZE.x + 15, 50};
}

void Disassembler::CPURegisters() {
#define DISPLAY_REGISTER(i, name)                                              \
    {                                                                          \
        m_CPURegisters.Registers[i].Value.SetText(to_hex(m_Bus.m_CPU.name));   \
        m_Renderer.DrawText(m_CPURegisters.Registers[i].Name, m_NextPosition); \
        m_NextPosition.x += 50;                                                \
        m_Renderer.DrawText(m_CPURegisters.Registers[i].Value,                 \
                            m_NextPosition);                                   \
        m_NextPosition = {NES_EMULATOR_SCREEN_SIZE.x + 15,                     \
                          m_NextPosition.y + 35};                              \
    }

    DISPLAY_REGISTER(0, a);
    DISPLAY_REGISTER(1, x);
    DISPLAY_REGISTER(2, y);
    DISPLAY_REGISTER(3, sp);
    DISPLAY_REGISTER(4, pc);

#undef DISPLAY_REGISTER
}

void Disassembler::MemoryDisassembly() {}

void Disassembler::PPUPalettes() {}

void Disassembler::PPUSprites() {}
