//
// created by Patrick Brukiewa - 18 Apr 15
//

#ifndef LUDUMDARE32_AICOMPONENT_HPP
#define LUDUMDARE32_AICOMPONENT_HPP

#include "entcom.hpp"
#include <functional>
#include <string>

class Engine;

void update_ais(Engine& engine, DB& db);

struct AIComponent {
    template <class T> AIComponent(T inputAi) { brain = inputAi; }
    std::function<void(Engine&, EntID, AIComponent &)> brain;

    void update(Engine& engine, EntID id) { brain(engine, id, *this); }

};

struct AiStateComponent {
    double vx;
    double vy;

    std::string anim;
    bool flipped;
};

#if 0
struct GoombaAI {
    void operator()(EntID me, AIComponent &myAi) {
        AiStateComponent curState;
        
        if (isFalling())
            curState = falling;
        else
            curState = walking;

        if (wallCollision()) {
            curState.flipped = !curState.flipped;
            if (curState.flipped)
                curState.vx = -1;
            else
                curState.vx = 1;
        } // end if

        db.makeComponent(me, curState);
    }

    AiStateComponent walking;
    AiStateComponent falling;

    GoombaAI() {
        walking.vx = 1;
        walking.vy = 0;
        walking.anim = "walking";
        walking.flipped = 0;

        falling.vx = 0;
        falling.vy = 1;
        falling.anim = "falling";
        falling.flipped = 0;
    } // end constructor
};
#endif

#endif // LUDUMDARE32_AICOMPONENT_HPP
