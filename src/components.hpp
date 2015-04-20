#ifndef LUDUMDARE32_COMPONENTS_HPP
#define LUDUMDARE32_COMPONENTS_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "AnimatedSprite.hpp"

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
    AnimatedSprite spr;
    int layer;

    Sprite() = default;
    Sprite(sf::Sprite sspr) {
        Animation anim;
        const sf::Texture *tex = sspr.getTexture();
        anim.addFrame(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(tex->getSize())));
        anim.setSpriteSheet(*tex);
        spr.setAnimation(anim);
    }
    Sprite(sf::Sprite sspr, int lyr) : Sprite(sspr) { layer = lyr; }
    Sprite(const sf::Texture& tex, sf::IntRect rect, int lyr) {
        Animation anim;
        anim.addFrame(rect);
        anim.setSpriteSheet(tex);
        spr.setAnimation(anim);
        layer = lyr;
    }
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
