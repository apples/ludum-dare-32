#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "thing.hpp"

struct Player : Thing {
    int pid;
    sf::RectangleShape rect;

    int y = 300;

    Player(int id) : pid(id), rect(sf::Vector2f(5,50)) {
        rect.setFillColor(sf::Color(255,255,255));
    }

    void draw(sf::RenderWindow& window) override {
        rect.setPosition(sf::Vector2f((pid==1? 20 : 800-20), y));
        window.draw(rect);
    }

    void tick() override {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            y -= 5;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            y += 5;
        }
        if (y<25) y=25;
        if (y>600-25) y=600-25;
    }
};

#endif // PLAYER_HPP
