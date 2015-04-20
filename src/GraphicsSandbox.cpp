//
// Created by xxAtrain223 on 4/18/2015.
//

#include <functional>
#include <iostream>
#include <map>
#include "GraphicsSandbox.hpp"
#include "components.hpp"
#include "Engine.hpp"
#include "LoadAnimation.hpp"
using namespace components;

struct TimerComponent {
    std::function<void()> func;
    double duration;

    template <class T> TimerComponent(T inputFunc, double inDur = 5) {
        func = inputFunc;
        duration = inDur;
    } // end constructor
}; // end TimerComponent

void updateTimers(DB & db, double timeStep) {
    auto items = db.query<TimerComponent>();

    for (auto & ent : items) {
        std::get<1>(ent).data().duration -= timeStep;

        if (std::get<1>(ent).data().duration <= 0)
            std::get<1>(ent).data().func();
    } // end for
} // end updateTimers

GraphicsSandbox::GraphicsSandbox() {
    if (!tileTex.loadFromFile("data/Brick.png")) {
        throw std::runtime_error("Failed to load data/Brick.png");
    }
    if (!playerTex.loadFromFile("data/Goomba.png")) {
        throw std::runtime_error("Failed to load data/Goomba.png");
    }

    texCache = TextureCache("data/textures.json");


    animations = loadAnimation("goomba.json", texCache);

    playerAnimSpr.setAnimation(animations["walk"]);
    playerAnimSpr.setFrameTime(sf::seconds(0.25f));
    playerAnimSpr.setLooped(true);
    playerAnimSpr.setScale(5.f, 5.f);

    sf::Vector2f tileSize = sf::Vector2f(tileTex.getSize());

    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            EntID eid = db.makeEntity();
            sf::Vector2f pos = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            sf::Sprite spr(texCache.get("brick"));
            spr.setPosition(pos);
            db.makeComponent(eid, Sprite{spr});
            db.makeComponent(eid, BoundingBox{sf::FloatRect{pos, tileSize}});
        }
    }

    cam.setMoveableArea(sf::FloatRect(sf::Vector2f(0, 0), tileSize * 50.f));
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

    playerAnimSpr.update(sf::seconds(time_step));
}

void GraphicsSandbox::draw(sf::RenderWindow &window) const {
    window.clear(sf::Color::Magenta);
    cam.draw(window, db);
    window.draw(playerAnimSpr);
}
