//
// Created by Jeramy on 4/19/2015.
//

#include "echo.hpp"
#include "TextureCache.hpp"

#include <json/json.h>

#include <fstream>

TextureCache::TextureCache(std::string fname) {
    Json::Value json;
    std::ifstream file (fname.c_str());
    file >> json;

    for (auto& spr : json.getMemberNames()) {
        load(spr, "data/"+json[spr].asString());
    }
}

void TextureCache::load(std::string name, std::string fname) {
    cache[name].loadFromFile(fname);
    ECHO("Loaded texture",name,"from",fname);
}

const sf::Texture& TextureCache::get(std::string name) const {
    auto iter = cache.find(name);
    if (iter == cache.end()) {
        throw std::runtime_error("Cannot find texture "+name+"!");
    }
    return iter->second;
}
