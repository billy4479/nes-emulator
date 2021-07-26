#include "AssetManager.hpp"
#include "Common.hpp"
#include <assert.h>
#include <stdexcept>

void AssetManager::LoadFont(const std::filesystem::path &path,
                            const std::string &name, u32 size) {
    auto font = TTF_OpenFont((m_AssetsFolder / path).string().c_str(), size);
    assert(font != nullptr);
    m_fonts.emplace(name, font);
}

TTF_Font *AssetManager::GetFont(const std::string &name) {
    auto font = m_fonts[name];
    assert(font != nullptr);
    return font;
}

AssetManager::~AssetManager() {
    ReleaseTTF();
    if (m_rom.is_open())
        m_rom.close();
}

void AssetManager::ReleaseTTF() {
    if (!m_fonts.empty()) {
        for (auto font : m_fonts) {
            if (font.second != nullptr) {
                TTF_CloseFont(font.second);
                font.second = nullptr;
            }
        }

        m_fonts.clear();
    }
}

void AssetManager::LoadROM(const std::filesystem::path &path) {
    if (m_rom.is_open())
        m_rom.close();
    m_rom.open(path, std::ios::binary | std::ios::ate);
    assert(m_rom.is_open());

    auto rom_size = m_rom.tellg();
    if (rom_size >= 0x8000) {
        m_rom.close();
        throw std::runtime_error("The ROM is too big!");
    }
    m_rom.seekg(0);
}

std::ifstream *AssetManager::GetROM() {
    if (m_rom.is_open())
        return &m_rom;
    return nullptr;
}