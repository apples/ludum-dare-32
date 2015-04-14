#ifndef THING_HPP
#define THING_HPP

#include <SFML/Window.hpp>
struct Thing {
    virtual void draw(sf::RenderWindow&) = 0;
    virtual void tick() = 0;
    virtual ~Thing() = 0;
};

inline Thing::~Thing() = default;

#endif // THING_HPP
