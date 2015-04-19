//
// Created by Jeramy on 4/18/2015.
//

#include "MainGame.hpp"
#include "components.hpp"
#include "Engine.hpp"

#include <json/json.h>

#include <fstream>

using components::Position;

struct TempSprite {
    sf::Color color;
};

MainGame::MainGame() {
    load_level("data/sandy.json");
}

MainGame::MainGame(Json::Value json) {
    load(json);
}

void MainGame::load_level(std::string fname) {
    Json::Value json;
    std::ifstream file (fname.c_str());
    file >> json;

    load(json);
}

void MainGame::load(Json::Value json) {
    entities = DB{};

    TempSprite sprites[] = {
            {{255,0,0}},
            {{0,255,0}},
            {{0,0,255}},
    };

    auto width = json["width"].asInt();
    auto height = json["height"].asInt();

    float y = 8;
    for (auto& row : json["rows"]) {
        float x = 8;
        for (auto& tile : row) {
            auto ent = entities.makeEntity();
            entities.makeComponent(ent, Position{{x,y}});
            entities.makeComponent(ent, sprites[tile.asInt()]);
            x += 16;
        }
        y += 16;
    }
}

bool MainGame::halts_update() const {
    return true;
}

bool MainGame::halts_draw() const {
    return true;
}

void MainGame::update(Engine &engine, double time_step) {
    if (engine.wasKeyPressed(sf::Keyboard::Escape)) {
        engine.states.pop();
    }
}

void MainGame::draw(sf::RenderWindow &window) const {
    using std::get;
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(16,16));
    rect.setOrigin(8,8);
    for (auto ent : entities.query<Position,TempSprite>()) {
        rect.setPosition(get<1>(ent).data().pos);
        rect.setFillColor(get<2>(ent).data().color);
        window.draw(rect);
    }
}
