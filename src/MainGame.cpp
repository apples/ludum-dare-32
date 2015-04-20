//
// Created by Jeramy on 4/18/2015.
//

#include "Ai.hpp"
#include "MainGame.hpp"
#include "components.hpp"
#include "Engine.hpp"
#include "PhysicsEngine.hpp"

#include <json/json.h>

#include <fstream>
#include <cstdlib>

using components::BoundingBox;
using components::Sprite;
using components::Solid;
using components::Velocity;

MainGame::MainGame() {
    load_level("data/sandy.json");
    cam.setSize(800,600);
}

MainGame::MainGame(Json::Value json) {
    load(json);
    cam.setSize(800,600);
}

void MainGame::load_level(std::string fname) {
    Json::Value json;
    std::ifstream file (fname.c_str());
    file >> json;

    load(json);
}

void MainGame::load(Json::Value json) {
    entities = DB{};

    auto& tex = texcache.get("terrain");
    auto sprW = tex.getSize().x/16.0;
    auto sprH = tex.getSize().y/16.0;
    std::vector<Sprite> terrasprites;
    terrasprites.reserve(256);
    for (int i=0; i<256; ++i) {
        Sprite sprite {{tex,sf::IntRect((i%16)*sprW,(i/16)*sprH,sprW,sprH)}, 0};
        terrasprites.push_back(sprite);
    }

    auto width = json["width"].asInt();
    auto height = json["height"].asInt();

    auto y = sprH/2.0;
    for (auto& row : json["rows"]) {
        auto x = sprW/2.0;
        for (auto& tile : row) {
            auto t = tile.asInt();
            if (t>0) {
                BoundingBox bb{{x, y, sprW, sprH}};
                Sprite spr = terrasprites[t];
                auto ent = entities.makeEntity();
                entities.makeComponent(ent, bb);
                entities.makeComponent(ent, spr);
                entities.makeComponent(ent, Solid{});
            }
            x += sprW;
        }
        y += sprH;
    }
    {
        Sprite player_sprite{{tex, sf::IntRect(sprW, 0, sprW, sprH)}, 1};
        BoundingBox player_bb{{64, 64+32, sprW, sprH}};
        Velocity player_vel{{50, 0}};
        AIComponent player_ai {[](Engine& engine, EntID me, AIComponent& my_ai){
            auto& vel = me.get<Velocity>().data();
            if (engine.isKeyDown(sf::Keyboard::Left)) {
                vel.acc.x += -2000;
            }
            if (engine.isKeyDown(sf::Keyboard::Right)) {
                vel.acc.x += 2000;
            }
            if (engine.wasKeyPressed(sf::Keyboard::Up)) {
                vel.timed_accs.push_back({{0,-40000},0.015});
            }
        }};

        auto player = entities.makeEntity();
        entities.makeComponent(player, player_sprite);
        entities.makeComponent(player, player_bb);
        entities.makeComponent(player, player_vel);
        entities.makeComponent(player, player_ai);
    }
}

bool MainGame::halts_update() const {
    return true;
}

bool MainGame::halts_draw() const {
    return true;
}

void MainGame::update(Engine &engine, double time_step) {
    update_ais(engine, entities);
    physics_step(entities, time_step);

    if (engine.wasKeyPressed(sf::Keyboard::Escape)) {
        engine.states.pop();
    }
}

void MainGame::draw(sf::RenderWindow &window) const {
    cam.draw(window, entities);
}
