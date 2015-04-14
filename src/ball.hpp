#ifndef BALL_HPP
#define BALL_HPP

#include "thing.hpp"

struct Ball : Thing {

    void draw(sf::RenderWindow&) override {}
    void tick() override {}
};

#endif // BALL_HPP
