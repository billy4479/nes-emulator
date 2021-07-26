#pragma once

#include "Bus.hpp"
namespace Emulation {
    class NES {
      public:
        NES();
        ~NES();
        inline std::string DumpRamAsHex(u16 start, u16 length) {
            return m_Bus.DumpRamAsHex(start, length);
        }

      private:
        Bus m_Bus;
    };
} // namespace Emulation