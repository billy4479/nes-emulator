#pragma once

#include "../Emulation/Bus.hpp"
#include "Label.hpp"
#include "Renderer.hpp"
#include "glm/ext/vector_int2.hpp"

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
    const Emulation::Bus& m_Bus;
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
};