#pragma once

#include <SDL2/SDL_ttf.h>

#include <filesystem>
#include <fstream>
#include <map>
#include <string>

#include "Common.hpp"

class AssetManager {
   public:
    AssetManager() = default;
    ~AssetManager();

    void LoadFont(const std::filesystem::path &, const std::string &, u32);
    TTF_Font *GetFont(const std::string &);
    void ReleaseTTF();

   private:
    const std::filesystem::path m_AssetsFolder{"assets"};
    std::map<const std::string, TTF_Font *> m_fonts;
};
