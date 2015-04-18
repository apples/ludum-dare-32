//
// Created by xxAtrain223 on 4/18/2015.
//

#include "GraphicsSandbox.hpp"
#include "components.hpp"
#include "Engine.hpp"
using namespace components;

GraphicsSandbox::GraphicsSandbox() {
    if (!tileTex.loadFromFile("data/Brick.png")) {
        throw std::runtime_error("Failed to load data/Brick.png");
    }

    sf::Vector2f tileSize = sf::Vector2f(tileTex.getSize());

    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            EntID eid = db.makeEntity();
            sf::Vector2f pos = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            sf::Sprite spr(tileTex);
            spr.setPosition(pos);
            db.makeComponent(eid, Sprite{spr});
            db.makeComponent(eid, BoundingBox{sf::FloatRect{pos, tileSize}});
        }
    }
}

bool GraphicsSandbox::halts_update() const {
    return true;
}

bool GraphicsSandbox::halts_draw() const {
    return true;
}

void GraphicsSandbox::update(Engine &engine, double time_step) {
    if (engine.wasKeyPressed(sf::Keyboard::Escape)) {
        engine.states.pop();
    }
}

void GraphicsSandbox::draw(sf::RenderWindow &window) const {
    window.clear(sf::Color(192, 192, 192, 255));
    for (auto& ent : db.query<Sprite, BoundingBox>()) {
        window.draw(std::get<1>(ent).data().spr);
    }
}