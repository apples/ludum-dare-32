//
// Created by Jeramy on 4/19/2015.
//

#ifndef LUDUMDARE32_LEVELEDITOR_H
#define LUDUMDARE32_LEVELEDITOR_H

#include "GameState.hpp"
#include "TextureCache.hpp"

#include <json/json.h>

#include <memory>

class LevelEditorSave : public GameState {
    sf::Font font;
public:
    std::string fname = "";
    LevelEditorSave();
    virtual bool halts_update() const override;
    virtual bool halts_draw() const override;
    virtual void update(Engine &engine, double time_step) override;
    virtual void draw(sf::RenderWindow &window) const override;
};

class LevelEditor : public GameState {
    Json::Value json;
    TextureCache textures = TextureCache("data/textures.json");

    std::shared_ptr<LevelEditorSave> saver;
public:
    LevelEditor();

    virtual bool halts_update() const override;
    virtual bool halts_draw() const override;
    virtual void update(Engine &engine, double time_step) override;
    virtual void draw(sf::RenderWindow &window) const override;

    int active_tile = 1;
};


#endif //LUDUMDARE32_LEVELEDITOR_H
