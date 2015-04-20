#ifndef LUDUMDARE32_COMPONENTS_HPP
#define LUDUMDARE32_COMPONENTS_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace components {

struct Position {
    sf::Vector2f pos;
};

struct Velocity {
    sf::Vector2f acc;
    sf::Vector2f vel;
    std::vector<std::pair<sf::Vector2f,double>> timed_accs;
};

struct Player {
    int id;
    float xpos;
};

struct Sprite {
    sf::Sprite spr;
    int layer;
};

struct BoundingBox {
    sf::FloatRect rect;
};

struct Solid {
};

struct Enemy {
};

} // namespace components

#include "components/ai.hpp"
#include "components/image.hpp"

#endif //LUDUMDARE32_COMPONENTS_HPP
