#pragma once

#include "Common.hpp"
#include <SDL2/SDL_ttf.h>
#include <filesystem>
#include <map>
#include <string>

class AssetManager {
  public:
    AssetManager() = default;
    ~AssetManager();

    void LoadFont(const std::filesystem::path &, const std::string &, u32);
    TTF_Font *GetFont(const std::string &);

  private:
    const std::filesystem::path m_AssetsFolder{"assets"};
    std::map<const std::string, TTF_Font *> m_fonts;
};
