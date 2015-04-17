#ifndef LUDUMDARE32_COMPONENTS_HPP
#define LUDUMDARE32_COMPONENTS_HPP

#include <SFML/System.hpp>

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

struct Ball {};

} // namespace components

#include "components/ai.hpp"
#include "components/image.hpp"

#endif //LUDUMDARE32_COMPONENTS_HPP
