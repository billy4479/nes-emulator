#pragma once

#include "Bus.hpp"
namespace Emulation {
    class NES {
      public:
        NES();
        ~NES();

      private:
        Bus bus;
    };
} // namespace Emulation