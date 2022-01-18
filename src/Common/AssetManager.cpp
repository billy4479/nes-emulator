#include "AssetManager.hpp"

#include <assert.h>

#include <stdexcept>

#include "Common.hpp"

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

AssetManager::~AssetManager() { ReleaseTTF(); }

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
