#ifndef LUDUMDARE32_AI_HPP
#define LUDUMDARE32_AI_HPP

#include "entcom.hpp"

namespace components {

struct AI {
    std::function<void(DB&,EntID)> brain;
};

struct PlayerBrain {
    std::function<bool()> move_up;
    std::function<bool()> move_down;

    void operator()(DB& db, EntID me) {
        auto& vel = me.get<Velocity>().data();
        if (move_up()) {
            vel.vel.y -= 5;
        }
        if (move_down()) {
            vel.vel.y += 5;
        }
    }
};

struct BallBrain {
    void operator()(DB& db, EntID me) {

    }
};

} // namespace components

#endif //LUDUMDARE32_AI_HPP
