//
// Created by Jeramy on 4/18/2015.
//

#ifndef LUDUMDARE32_MAINGAME_H
#define LUDUMDARE32_MAINGAME_H

#include "GameState.hpp"
#include "entcom.hpp"
#include "TextureCache.hpp"
#include "Camera.hpp"

#include <json/json.h>

#include <string>

class MainGame : public GameState {
    mutable DB entities;
    TextureCache texcache = TextureCache("data/textures.json");
    std::map<std::string, std::map<std::string, Animation>> animations;
    AnimatedSprite playerAnimSpr;
    Camera cam;
    EntID player;

    struct TerrainData {
        bool is_solid = false;
    };
    std::vector<TerrainData> terrain_data = std::vector<TerrainData>(256);

public:
    MainGame();
    MainGame(Json::Value json);
    void load_level(std::string fname);
    void load(Json::Value json);

    virtual bool halts_update() const override;
    virtual bool halts_draw() const override;
    virtual void update(Engine &engine, double time_step) override;
    virtual void draw(sf::RenderWindow &window) const override;

    void update_animations();
};


#endif //LUDUMDARE32_MAINGAME_H
