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
    std::string prompt;
public:
    std::string fname = "";
    LevelEditorSave(std::string prompt);
    virtual bool halts_update() const override;
    virtual bool halts_draw() const override;
    virtual void update(Engine& engine, double time_step) override;
    virtual void draw(sf::RenderWindow& window) const override;
};

class LevelEditorPick : public GameState {
    int* active_tile = nullptr;
    const sf::Texture* tex = nullptr;
public:
    LevelEditorPick(int* active_tile, const sf::Texture* tex);
    virtual bool halts_update() const override;
    virtual bool halts_draw() const override;
    virtual void update(Engine& engine, double time_step) override;
    virtual void draw(sf::RenderWindow& window) const override;
};

class LevelEditor : public GameState {
    Json::Value json;
    TextureCache textures = TextureCache("data/textures.json");
    sf::Font font;

    std::shared_ptr<LevelEditorSave> saver;
    std::shared_ptr<LevelEditorSave> loader;

    std::string fname = "";

    sf::Vector2f camoffset;
public:
    LevelEditor();

    virtual bool halts_update() const override;
    virtual bool halts_draw() const override;
    virtual void update(Engine& engine, double time_step) override;
    virtual void draw(sf::RenderWindow& window) const override;

    int active_tile = 1;
};


#endif //LUDUMDARE32_LEVELEDITOR_H
