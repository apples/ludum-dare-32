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
    Camera cam;
public:
    MainGame();
    MainGame(Json::Value json);
    void load_level(std::string fname);
    void load(Json::Value json);

    virtual bool halts_update() const override;
    virtual bool halts_draw() const override;
    virtual void update(Engine &engine, double time_step) override;
    virtual void draw(sf::RenderWindow &window) const override;
};


#endif //LUDUMDARE32_MAINGAME_H
