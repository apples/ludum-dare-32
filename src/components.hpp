#ifndef LUDUMDARE32_COMPONENTS_HPP
#define LUDUMDARE32_COMPONENTS_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Animation.hpp"

namespace components {

struct Position {
    sf::Vector2f pos;
};

struct Velocity {
    sf::Vector2f vel;
};

struct Player {
    int id;
    float xpos;
};

struct Sprite {
    Animation spr;
    int layer = 0;

    Sprite() = default;
    Sprite(sf::Sprite s) {spr = Animation(s);}
};

struct BoundingBox {
    sf::FloatRect rect;
};

} // namespace components

#include "components/ai.hpp"
#include "components/image.hpp"

#endif //LUDUMDARE32_COMPONENTS_HPP
