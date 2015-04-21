#ifndef LUDUMDARE32_COMPONENTS_HPP
#define LUDUMDARE32_COMPONENTS_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "AnimatedSprite.hpp"
#include "entcom.hpp"

namespace components {

struct AiStateComponent {
    std::string anim_name;
    std::string anim_group;
    bool flipped;
}; // end AiStateComponent

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
    int layer = 0;
    bool flipped = false;
    AiStateComponent prev_state;

    Sprite() = default;
    Sprite(sf::Sprite sspr) {
        Animation anim;
        const sf::Texture *tex = sspr.getTexture();
        anim.addFrame(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(tex->getSize())));
        anim.setSpriteSheet(*tex);
        spr.setAnimation(anim);
    }
    Sprite(sf::Sprite sspr, int lyr) : Sprite(sspr) { layer = lyr; }
    Sprite(AnimatedSprite sspr, int lyr) : spr(sspr) { layer = lyr; }
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

struct LookAt {
    EntID target;
};

struct PainBox {
    enum class Team {
        PLAYER,
        HELLBEASTS
    };
    Team team;
};

struct LockInput {
};

struct Solid {
};

struct NoCollide {
};

struct Enemy {
};

struct TimerComponent {
    std::function<void()> func;
    double duration;
}; // end TimerComponent

inline void update_timers(DB & db, double timeStep) {
    auto items = db.query<TimerComponent>();

    for (auto & ent : items) {
        std::get<1>(ent).data().duration -= timeStep;

        if (std::get<1>(ent).data().duration <= 0)
            std::get<1>(ent).data().func();
    } // end for
} // end updateTimers

} // namespace components

#include "components/ai.hpp"
#include "components/image.hpp"

#endif //LUDUMDARE32_COMPONENTS_HPP
