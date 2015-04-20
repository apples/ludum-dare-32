//
// Created by xxAtrain223 on 4/18/2015.
//

#include "GraphicsSandbox.hpp"
#include "components.hpp"
#include "Engine.hpp"
#include "Animation.hpp"
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
            spr.setTextureRect(sf::IntRect(sf::Vector2i(16, 16), sf::Vector2i(16, 16)));
            db.makeComponent(eid, Sprite{spr});
            db.makeComponent(eid, BoundingBox{sf::FloatRect{pos, tileSize}});
        }
    }

    cam.setMoveableArea(sf::FloatRect(sf::Vector2f(0, 0), tileSize * 50.f));

    if (!playerTex.loadFromFile("data/girl.png")) {
        throw std::runtime_error("Failed to load data/girl.png");
    }

    Animation playerAnim("data/player.json", playerTex);
    playerAnim.setState("walk");
    EntID eid = db.makeEntity();
    db.makeComponent(eid, Sprite{playerAnim});
    db.makeComponent(eid, BoundingBox{sf::FloatRect{sf::Vector2f(0, 0), sf::Vector2f(32, 64)}});
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
        return;
    }

    if (cam.getSize() == sf::Vector2f(0, 0)) {
        cam.setSize(sf::Vector2f(engine.window.getSize()));
    }

    sf::Vector2f delta(0, 0);

    if (engine.isKeyDown(sf::Keyboard::W) || engine.isKeyDown(sf::Keyboard::Up)) {
        --delta.y;
    }
    if (engine.isKeyDown(sf::Keyboard::S) || engine.isKeyDown(sf::Keyboard::Down)) {
        ++delta.y;
    }
    if (engine.isKeyDown(sf::Keyboard::A) || engine.isKeyDown(sf::Keyboard::Left)) {
        --delta.x;
    }
    if (engine.isKeyDown(sf::Keyboard::D) || engine.isKeyDown(sf::Keyboard::Right)) {
        ++delta.x;
    }

    cam.move(delta * 10.f);

    auto items = db.query<Sprite>();
    for (auto& ent : items) {
        std::get<1>(ent).data().spr.update(engine, time_step);
    }
}

void GraphicsSandbox::draw(sf::RenderWindow &window) const {
    window.clear(sf::Color::Magenta);
    cam.draw(window, db);
}