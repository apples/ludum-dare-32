//
// created by Patrick Brukiewa - 18 Apr 15
//

#ifndef LUDUMDARE32_AICOMPONENT_HPP
#define LUDUMDARE32_AICOMPONENT_HPP

#include <functional>
#include <iostream>
#include "entcom.hpp"

struct AIComponent {
    template <class T> AIComponent(T inputAi) { brain = inputAi; }
    std::function<void(EntID, AIComponent &)> brain;

    void update(EntID id) { brain(id, *this); }

};

struct StateComponent {
    double vx;
    double vy;

    string anim;
    bool flipped;
};

struct GoombaAI {
    void operator()(EntID me, AIComponent &myAi) {
        StateComponent curState;
        
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

    StateComponent walking;
    StateComponent falling;

    GoombaAI::GoombaAI() {
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

#endif // LUDUMDARE32_AICOMPONENT_HPP
