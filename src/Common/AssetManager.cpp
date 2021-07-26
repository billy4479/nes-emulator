#include "AssetManager.hpp"
#include "Common.hpp"
#include <assert.h>

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
    for (auto font : m_fonts) {
        TTF_CloseFont(font.second);
    }
}