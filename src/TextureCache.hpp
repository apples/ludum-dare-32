//
// Created by Jeramy on 4/19/2015.
//

#ifndef LUDUMDARE32_TEXTURECACHE_HPP
#define LUDUMDARE32_TEXTURECACHE_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>

class TextureCache {
    std::unordered_map<std::string,sf::Texture> cache;
public:
    TextureCache() = default;
    TextureCache(std::string fname);

    void load(std::string name, std::string fname);
    const sf::Texture& get(std::string name) const;
};


#endif //LUDUMDARE32_TEXTURECACHE_HPP
