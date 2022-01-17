#pragma once
#include "Mapper.hpp"

namespace Emulation {
namespace Mappers {

class Mapper000 : public Mapper {
   public:
    Mapper000(u8 prgBanks, u8 chrBanks) : Mapper(prgBanks, chrBanks) {}
    ~Mapper000() = default;

    bool CPUMapRead(u16 addr, u32 &mappedAddr) override;
    bool CPUMapWrite(u16 addr, u32 &mappedAddr) override;

    bool PPUMapRead(u16 addr, u32 &mappedAddr) override;
    bool PPUMapWrite(u16 addr, u32 &mappedAddr) override;
};

}  // namespace Mappers
}  // namespace Emulation