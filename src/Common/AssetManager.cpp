#include "AssetManager.hpp"

#include <SDL_ttf.h>
#include <linux/limits.h>
#include <unistd.h>

#include <stdexcept>

#include "Common.hpp"

AssetManager::AssetManager(std::filesystem::path assetFolder) {
    SetAssetFolder(assetFolder);
}

TTF_Font *AssetManager::LoadFont(const std::filesystem::path &path,
                                 const std::string &name, u32 size) {
    auto font = TTF_OpenFont((m_AssetsFolder / path).c_str(), size);
    ASSERT(font != nullptr);

    [[maybe_unused]] auto result = m_Fonts.emplace(name, font);
    ASSERT(result.second);

    return font;
}

TTF_Font *AssetManager::GetFont(const std::string &name) {
    auto font = m_Fonts[name];
    ASSERT(font != nullptr);
    return font;
}

std::filesystem::path AssetManager::LoadROM(std::filesystem::path path) const {
    return m_AssetsFolder / path;
}

AssetManager::~AssetManager() { ReleaseSDLDependencies(); }

void AssetManager::SetAssetFolder(const std::filesystem::path &path) {
    if (path.is_relative())
        m_AssetsFolder = m_BasePath / path;
    else
        m_AssetsFolder = path;
}

std::filesystem::path AssetManager::GetAssetFolder() const {
    return m_AssetsFolder;
}

void AssetManager::ReleaseSDLDependencies() {
    for (auto f : m_Fonts) {
        TTF_CloseFont(f.second);
    }
    m_Fonts.clear();
}

const std::filesystem::path AssetManager::GetBasePath() const {
// TODO: Check if works on windows
#ifdef BE_PLATFORM_WINDOWS
    wchar_t path[MAX_PATH] = {0};
    GetModuleFileNameW(NULL, path, MAX_PATH);
    return std::filesystem::path(path).parent_path();
#else
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    auto p = std::string(result, (size_t)((count > 0) ? count : 0));
    return std::filesystem::path(p).parent_path();
#endif
}
