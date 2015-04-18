//
// Created by Jeramy on 4/18/2015.
//

#ifndef LUDUMDARE32_ENGINE_HPP
#define LUDUMDARE32_ENGINE_HPP

#include "entcom.hpp"
#include "GameStateStack.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Engine {
    DB db;
    EntID player;
    GameStateStack states;
    sf::RenderWindow window;

    void poll_events();
    void update();
    void draw();

public:
    Engine();
    void go();
};


#endif //LUDUMDARE32_ENGINE_HPP
