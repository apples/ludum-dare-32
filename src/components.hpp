#ifndef LUDUMDARE32_COMPONENTS_HPP
#define LUDUMDARE32_COMPONENTS_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics/Sprite.hpp>

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
    sf::Sprite spr;
};

struct BoundingBox {
    sf::FloatRect rect;
};

} // namespace components

#include "components/ai.hpp"
#include "components/image.hpp"

#endif //LUDUMDARE32_COMPONENTS_HPP
