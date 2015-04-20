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

    std::string animation;
    bool flipped;

    void setAll(double x, double y, std::string anim, bool f) {
        vx = x;
        vy = y;
        animation = anim;
        flipped = f;
    } // end setAll
}; // end AiStateComponent

#if 0
struct PlayerAI {
    double walkVelocity;
    double fallVelocity;
    double knockbackVelocity;
    double jumpVelocity;

    AiStateComponent idle;
    AiStateComponent walkingLeft;
    AiStateComponent walkingRight;
    AiStateComponent fallingLeft;
    AiStateComponent fallingRight;
    AiStateComponent fallingDown;
    AiStateComponent jumpingLeft;
    AiStateComponent jumpintRight;
    AiStateComponent jumpingUp;
    AiStateComponent knockbackLeft;
    AiStateComponent knockbackRight;
    AiStateComponent dying;
    AiStateComponent summoning;
    AiStateComponent dismissing;

    PlayerAI(double, double, double, double);

    void operator()(EntID, AIComponent &);
}; // end PlayerAI
#endif
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
