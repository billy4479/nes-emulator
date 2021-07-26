#pragma once
#include "../Common/Common.hpp"
#include <filesystem>
#include <memory>
#include <vector>

#include "Mappers/MapperInclude.hpp"

namespace Emulation {
    class Cartridge {
      public:
        Cartridge(std::filesystem::path);
        ~Cartridge() = default;

        bool CPURead(u16 addr, u8 &data);
        bool CPUWrite(u16 addr, u8 data);

        bool PPURead(u16 addr, u8 &data);
        bool PPUWrite(u16 addr, u8 data);

      private:
        std::vector<u8> m_PRGMemory;
        std::vector<u8> m_CHRMemory;

        std::shared_ptr<Mappers::Mapper> m_Mapper;

        u8 m_MapperID = 0;
        u8 m_PRGBanks = 0;
        u8 m_CHRBanks = 0;
    };
} // namespace Emulation