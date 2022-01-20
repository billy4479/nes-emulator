#include "Cartridge.hpp"

#include <assert.h>

#include <fstream>
#include <ios>
#include <memory>

namespace Emulation {

Cartridge::Cartridge(std::filesystem::path path) {
    struct Header {
        // This is contains 'NAS\r'
        char Name[4];

        // Size of ProgramROM * 16KB
        uint8_t ProgramRomChunks;
        // Size of PatternROM * 8KB
        // 0 means it uses Pattern RAM
        uint8_t PatternRomChunks;

        uint8_t MapperFlags_1;
        uint8_t MapperFlags_2;

        // Rarely used
        uint8_t ProgramRamSize;

        // TV system, rarely used
        uint8_t tv_system1;
        uint8_t tv_system2;  // This is unofficial

        // Padding
        char Unused[5];
    } header;

    std::ifstream file;
    file.open(path, std::ifstream::binary);
    assert(file.is_open());

    // Read the header
    file.read(reinterpret_cast<char *>(&header), sizeof(Header));

    if (header.MapperFlags_1 & 0x04) {
        // This bit indicates if the catridge contains a trainer.
        // If it does we skip it
        file.seekg(512, std::ios_base::cur);
    }

    m_MapperID =
        ((header.MapperFlags_2 >> 4) << 4 | (header.MapperFlags_1 >> 4));

    // TODO: Detect and support V2 and V0
    // https://wiki.nesdev.com/w/index.php?title=INES

    u8 version = 1;

    switch (version) {
        case 0:
            assert(false);
        case 1:
            m_ProgramMemoryBanks = header.ProgramRomChunks;
            m_PRGMemory.resize(m_ProgramMemoryBanks * 16 * 1024);
            file.read(reinterpret_cast<char *>(m_PRGMemory.data()),
                      m_PRGMemory.size());

            m_PatternMemoryBanks = header.PatternRomChunks;
            m_CHRMemory.resize(m_PatternMemoryBanks * 8 * 1024);
            file.read(reinterpret_cast<char *>(m_CHRMemory.data()),
                      m_CHRMemory.size());
            break;
        case 2:
            assert(false);
        default:
            assert(false);
    }

    file.close();

    // Load Mapper

#define MAPPER_CASE(id, code)                               \
    case id:                                                \
        m_Mapper = std::make_shared<Mappers::Mapper##code>( \
            m_ProgramMemoryBanks, m_PatternMemoryBanks);    \
        break;

    // Load the correct mapper based on it's ID
    switch (m_MapperID) {
        MAPPER_CASE(0, 000)
        default:
            assert(false);
    }

#undef MAPPER_CASE
}

bool Cartridge::CPURead(u16 addr, u8 &data) {
    u32 mappedAddr = 0;
    if (m_Mapper->CPUMapRead(addr, mappedAddr)) {
        data = m_PRGMemory[mappedAddr];
        return true;
    }
    return false;
}

bool Cartridge::CPUWrite(u16 addr, u8 data) {
    u32 mappedAddr = 0;
    if (m_Mapper->CPUMapRead(addr, mappedAddr)) {
        m_PRGMemory[mappedAddr] = data;
        return true;
    }
    return false;
}

bool Cartridge::PPURead(u16 addr, u8 &data) {
    u32 mappedAddr = 0;
    if (m_Mapper->CPUMapRead(addr, mappedAddr)) {
        data = m_CHRMemory[mappedAddr];
        return true;
    }
    return false;
}

bool Cartridge::PPUWrite(u16 addr, u8 data) {
    u32 mappedAddr = 0;
    if (m_Mapper->CPUMapRead(addr, mappedAddr)) {
        m_CHRMemory[mappedAddr] = data;
        return true;
    }
    return false;
}

}  // namespace Emulation