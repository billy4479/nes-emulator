#pragma once
#include "../../Common/Common.hpp"

// https://wiki.nesdev.com/w/index.php?title=Mapper

namespace Emulation {
namespace Mappers {

class Mapper {
   public:
    Mapper(u8 prgBanks, u8 chrBanks)
        : m_PRGBanks(prgBanks), m_CHRBanks(chrBanks) {}
    ~Mapper() = default;

    virtual bool CPUMapRead(u16 addr, u32 &mappedAddr) = 0;
    virtual bool CPUMapWrite(u16 addr, u32 &mappedAddr) = 0;
    virtual bool PPUMapRead(u16 addr, u32 &mappedAddr) = 0;
    virtual bool PPUMapWrite(u16 addr, u32 &mappedAddr) = 0;

   protected:
    u8 m_PRGBanks = 0;
    u8 m_CHRBanks = 0;
};

}  // namespace Mappers
}  // namespace Emulation