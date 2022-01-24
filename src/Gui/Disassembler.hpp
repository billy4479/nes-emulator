#pragma once

#include <cstdio>

#include "../Emulation/Bus.hpp"
#include "Label.hpp"
#include "Renderer.hpp"
#include "glm/ext/vector_int2.hpp"

namespace Emulation {

class Disassembler {
   public:
    Disassembler(const Emulation::Bus&, GUI::Renderer&, AssetManager&);
    void Render();

   private:
    void CPURegisters();
    void CPUStatus();
    void MemoryDisassembly();
    void PPUPalettes();
    void PPUSprites();

   private:
    static constexpr i32 charWidth = 15;
    static constexpr i32 charHeight = 30;
    static constexpr i32 leftPadding = 15;

    const Bus& m_Bus;
    GUI::Renderer& m_Renderer;
    TTF_Font* m_Font;

    glm::ivec2 m_NextPosition;

    struct {
        struct {
            GUI::Label StatusLabel;
            GUI::Label Bits[8];
        } Status;

        struct {
            GUI::Label Name;
            GUI::Label Value;
        } Registers[5];
    } m_CPURegisters;

   private:
    std::map<u16, std::string> Disassemble(u16 start, u16 end, const Bus& bus);
    void NextLine();
};
}  // namespace Emulation