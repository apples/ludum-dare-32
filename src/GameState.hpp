//
// Created by Jeramy on 4/18/2015.
//

#ifndef LUDUMDARE32_GAMESTATE_H
#define LUDUMDARE32_GAMESTATE_H

#include <SFML/Graphics.hpp>

class Engine;

class GameState {
public:
    virtual bool halts_update() const { return false; };
    virtual bool halts_draw() const { return false; };
    virtual void update(Engine& engine, double time_step) { };
    virtual void draw(sf::RenderWindow &window) const { };
    virtual ~GameState() = 0;
};

inline GameState::~GameState() = default;


#endif //LUDUMDARE32_GAMESTATE_H
