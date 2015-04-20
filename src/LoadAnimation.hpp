//
// Created by xxAtrain223 on 4/19/2015.
//

#ifndef LUDUMDARE32_LOADANIMATION_HPP
#define LUDUMDARE32_LOADANIMATION_HPP

#include "Animation.hpp"
#include "AnimatedSprite.hpp"
#include "TextureCache.hpp"

#include <sfml/Graphics.hpp>
#include <json/json.h>

#include <map>
#include <fstream>

std::map<std::string, Animation> loadAnimation(std::string jsonFileName, TextureCache& texCache) {
    Json::Value json;
    std::ifstream file("data/player.json");
    file >> json;

    std::map<std::string, Animation> rv;

    sf::Vector2i frameSize = sf::Vector2i(json["width"].asInt(), json["height"].asInt());
    for (auto& name : json.getMemberNames()) {
        if (name == "spritesheet" || name == "width" || name == "height") {
            continue;
        }
        Json::Value animations = json[name];
        Json::Value frames = animations["frames"];
        Animation animation;
        for (Json::Value f : frames) {
            int r = f["row"].asInt();
            int c = f["col"].asInt();
            animation.addFrame(sf::IntRect(sf::Vector2i(c * frameSize.x, r * frameSize.y), frameSize));
        }
        animation.setSpriteSheet(texCache.get(json["spritesheet"].asString()));
        rv.insert(make_pair(name, animation));
    }

    return rv;
}

#endif //LUDUMDARE32_LOADANIMATION_HPP
