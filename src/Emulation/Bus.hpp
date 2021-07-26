#pragma once

#include "../Common/Common.hpp"
#include "CPU.hpp"
#include <array>
#include <stdexcept>

namespace Emulation {
    class Bus {
      public:
        Bus();
        ~Bus();

        /*  *** Devices *** */
        CPU cpu;
        std::array<u8, 64 * 1024> ram; // This is going to change

        /*  *** Read & Write *** */
        void Write(u16 addr, u8 data);
        u8 Read(u16 addr, bool readOnly = false);

      private:
    };
} // namespace Emulation