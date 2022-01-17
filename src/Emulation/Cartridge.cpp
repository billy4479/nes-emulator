#include "Cartridge.hpp"

#include <assert.h>

#include <fstream>
#include <ios>
#include <memory>

namespace Emulation {

Cartridge::Cartridge(std::filesystem::path path) {
    struct Header {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    std::ifstream ifs;
    ifs.open(path, std::ifstream::binary);
    assert(ifs.is_open());

    ifs.read((char *)&header, sizeof(Header));
    if (header.mapper1 & 0x04) ifs.seekg(512, std::ios_base::cur);
    m_MapperID = ((header.mapper2 >> 4) << 4 | (header.mapper1 >> 4));

    // TODO: Detect and support V2 and V0
    // https://wiki.nesdev.com/w/index.php?title=INES

    u8 version = 1;

    switch (version) {
        case 0:
            assert(false);
        case 1:
            m_PRGBanks = header.prg_rom_chunks;
            m_PRGMemory.resize(m_PRGBanks * 16 * 1024);
            ifs.read((char *)m_PRGMemory.data(), m_PRGMemory.size());

            m_CHRBanks = header.chr_rom_chunks;
            m_CHRMemory.resize(m_CHRBanks * 8 * 1024);
            ifs.read((char *)m_CHRMemory.data(), m_CHRMemory.size());
            break;
        case 2:
            assert(false);
        default:
            assert(false);
    }

    ifs.close();

    // Load Mapper

    switch (m_MapperID) {
        case 0:
            m_Mapper =
                std::make_shared<Mappers::Mapper000>(m_PRGBanks, m_CHRBanks);
            break;
        default:
            assert(false);
    }
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