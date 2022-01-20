#pragma once

#include <SDL2/SDL_ttf.h>
#include <SDL_surface.h>

#include <filesystem>
#include <fstream>
#include <map>
#include <string>

#include "Common.hpp"

class AssetManager {
   public:
    explicit AssetManager(std::filesystem::path assetFolder = "");
    ~AssetManager();

    void SetAssetFolder(const std::filesystem::path &);
    std::filesystem::path GetAssetFolder() const;

    TTF_Font *LoadFont(const std::filesystem::path &, const std::string &, u32);
    TTF_Font *GetFont(const std::string &);

    std::filesystem::path LoadROM(std::filesystem::path) const;

    void ReleaseSDLDependencies();

   private:
    // https://stackoverflow.com/questions/1528298/get-path-of-executable
    const std::filesystem::path GetBasePath() const;

    std::filesystem::path m_AssetsFolder{""};
    const std::filesystem::path m_BasePath = GetBasePath();
    std::map<const std::string, TTF_Font *> m_Fonts{};
};
