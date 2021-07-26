#pragma once
#include "Mapper.hpp"

namespace Emulation::Mappers {
    class Mapper000 : public Mapper {
        bool CPUMapRead(u16 addr, u32 &mappedAddr) override {
            if (addr >= 0x8000 && addr <= 0xFFFF) {
                mappedAddr = addr & (m_PRGBanks > 1 ? 0x7FFF : 0x3FFF);
                return true;
            }

            return false;
        }

        bool CPUMapWrite(u16 addr, u32 &mappedAddr) override {
            if (addr >= 0x8000 && addr <= 0xFFFF) {
                mappedAddr = addr & (m_PRGBanks > 1 ? 0x7FFF : 0x3FFF);
                return true;
            }

            return false;
        }

        bool PPUMapRead(u16 addr, u32 &mappedAddr) override {
            if (addr >= 0x8000 && addr <= 0x1FFF) {
                mappedAddr = addr;
                return true;
            }

            return false;
        }

        bool PPUMapWrite(u16 addr, u32 &mappedAddr) override {
            // if (addr >= 0x8000 && addr <= 0x1FFF)
            //     return true;

            return false;
        }
    };
} // namespace Emulation::Mappers