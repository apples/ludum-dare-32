//
// created by Patrick Brukiewa - 18 Apr 15
//

#include "Ai.hpp"

void update_ais(Engine& engine, DB& db) {
    for (auto ent : db.query<AIComponent>()) {
        std::get<1>(ent).data().update(engine, std::get<0>(ent));
    }
}
#if 0
PlayerAI::PlayerAI(double wv, double fv, double kv, double jv) {
    walkVelocity = wv;
    fallVelocity = fv;
    knockbackVelocity = kv;
    jumpVelocity = jv;

    idle.setAll(0, 0, "idle", 0);
    walkingLeft.setAll(-walkVelocity, 0, "walking", 1);
    walkingRight.setAll(walkVelocity, 0, "walking", 0);
    fallingLeft.setAll(-walkVelocity, fallVelocity, "falling", 1);
    fallingRight.setAll(walkVelocity, fallVelocity, "falling", 0);
    fallingDown.setAll(0, fallVelocity, "falling", 0);
    jumpingLeft.setAll(-walkVelocity, jumpVelocity, "jumping", 1);
    jumpingRight.setAll(walkVelocity, jumpVelocity, "jumping", 0);
    jumpingUp.setAll(0, jumpVelocity, "jumping", 0);
    knockbackLeft(-knockbackVelocity, 0, "knockback", 1);
    knockbackRight(knockbackVelocity, 0, "knockback", 0);
    dying(0, 0, "dying", 0);
    summoning(0, 0, "summoning", 0);
    dismissing(0, 0, "dismissing", 0);
} // end constructor

void PlayerAI::operator()(EntID me, AIComponent &myAi) {
    if (isIdle()) {
        if (leftKeyPressed)
            setState(walkingLeft);

        else if (rightKeyPressed)
            setState(walkingRight);

        else if (spaceKeyPressed)
            setState(jumpingUp);

        else if (dKeyPressed) {
            if (summoned)
                setState(dismissing);
            else
                setState(summoning);
        } // end else if

        else if (enemyCollisionLeft()) {
            setState(knockbackRight);
            reduceHealth();
        } // end else if

        else if (enemyCollisionRight()) {
            setState(knockbackLeft);
            reduceHealth();
        }
    } // end if (idle)

    else if (isWalkingLeft()) {
        if (leftKeyReleased)
            setState(idle);

        else if (!collisionFloor())
            setState(fallingLeft);

        else if (spaceKeyPressed)
            setState(jumpingLeft);

        else if (rightKeyPressed)
            setState(walkingRight);

        else if (enemyCollisionLeft()) {
            setState(knockbackRight);
            reduceHealth();
        } // end else if

        else if (enemyCollisionRight()) {
            setState(knockbackLeft);
            reduceHealth();
        } // end else if

        else if (dKeyPressed) {
            if (summoned)
                setState(dismissing);
            else
                setState(summoning);
        } // end else if
} // end operator()

void PlayerAI::operator()(EntID me, AIComponent & myAi) {
    if (leftKeyPressed) {
        if (isIdle() || isWalkingRight())
            setState(walkingLeft);

        else if (isFallingDown() || isFallingRight())
            setState(fallingLeft);

        else if (isJumpingRight() || isJumpingUp())
            setState(jumpingLeft);
    } // end if

    else if (rightKeyPressed) {
        if (isIdle() || isWalkingLeft())
            setState(walkingRight);

        else if (isFallingDown() || isFallingLeft())
            setState(fallingRight);

        else if (isJumpingLeft() || isJumpingUp())
            setState(jumpingRight):
    } // end else if

    else if (spaceKeyPressed) {
        if (isIdle())
            setState(jumpingUp);

        else if (isWalkingLeft)
            setState(jumpingLeft);

        else if (isWalkingRight())
            setState(jumpingRight);
    } // end else if
} // end operator()
#endif
