//
// created by Patrick Brukiewa - 18 Apr 15
//

#ifndef LUDUMDARE32_AICOMPONENT_HPP
#define LUDUMDARE32_AICOMPONENT_HPP

#include "entcom.hpp"
#include "PhysicsEngine.hpp"
#include "components.hpp"
#include <functional>
#include <string>
#include <cstdlib>
#include <memory>
#include <cmath>

using components::BoundingBox;
using components::Sprite;
using components::Velocity;
using components::Enemy;
using components::Solid;

class Engine;

void update_ais(Engine& engine, DB& db);

struct AIComponent {
    template <class T> AIComponent(T inputAi) { brain = inputAi; }
    std::function<void(Engine&, DB&, EntID, AIComponent &)> brain;

    void update(Engine& engine, DB& db, EntID id) { brain(engine, db, id, *this); }

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

struct PlayerAI {
    std::function<EntID(EntID)> make_bear;
    void operator()(Engine& engine, DB&, EntID me, AIComponent &my_ai);
}; // end PlayerAI

struct PlayerAIWithBear {
    EntID bear;
    std::function<EntID(EntID)> make_bear;
    void operator()(Engine& engine, DB&, EntID me, AIComponent &my_ai);
}; // end PlayerAI

struct PlayerAIIdle {
    void operator()(Engine& engine, DB&, EntID me, AIComponent &my_ai);
};

struct PlayerBearAI {
    EntID player;
    std::function<EntID(EntID)> make_bear;
    void operator()(Engine& engine, DB&, EntID me, AIComponent &my_ai);
};

struct PlayerBearAIIdle {
    void operator()(Engine& engine, DB&, EntID me, AIComponent &my_ai);
};

struct BearAI {
    DB *database;
    bool hitLeftRight;
    bool hitDown;
    bool shouldJump;
    double maxVel;

    BearAI(DB *input_db) {
        database = input_db;
        maxVel = 2000;
    };

    void operator()(Engine& engine, EntID me, AIComponent& myAi) {
        auto& vel = me.get<Velocity>().data();
        auto colinfo = me.get<CollisionData>();
        std::unique_ptr<EntID> target;
        double distance = 500;
        double tmpDist;
        double myX, myY, enemyX, enemyY, targetX, targetY;

        hitLeftRight = false;
        hitDown = false;

        auto enemies = database->query<Enemy>();
        
        myX = me.get<BoundingBox>().data().rect.left;
        myY = me.get<BoundingBox>().data().rect.top;

        for (auto & ent : enemies) {
            enemyX = std::get<0>(ent).get<BoundingBox>().data().rect.left;
            enemyY = std::get<0>(ent).get<BoundingBox>().data().rect.top;

            tmpDist = sqrt((enemyX - myX) * (enemyX - myX) + (enemyY - myY) * (enemyY - myY));

            if (tmpDist < distance) {
                distance = tmpDist;
                target = std::make_unique<EntID>(std::get<0>(ent));
                targetX = enemyX;
                targetY = enemyY;
            } // end if
        } // end for

        if (colinfo) {
            auto& coldata = colinfo.data();

            for (auto& hit : coldata.hits) {
                if (hit.dir == CollisionData::HitDir::RIGHT || hit.dir == CollisionData::HitDir::LEFT)
                    hitLeftRight = true;
                if (hit.dir == CollisionData::HitDir::DOWN)
                    hitDown = true;
            } // end for
        } // end if

        if (target) {
            if (hitLeftRight)
                vel.acc.x *= -1.0 / 2;
            if (targetX < myX && -vel.vel.x < maxVel)
                vel.acc.x -= 2000;
            if (targetX > myX && vel.vel.x < maxVel)
                vel.acc.x += 2000;
            if (!hitDown)
                vel.acc.y += 4000;
        } // end if

        else {
        } // end else
                
    } // end operator()
}; // end BearAI
    

struct GoombaAI {
    DB *database;
    bool movingRight;
    bool hitLeftRight;
    bool onFloor;
    bool shouldJump;
    double maxVel;

    GoombaAI(DB *input_db) {
        database = input_db;
        movingRight = true;
        maxVel = 2000;
    } // end constructor

    void operator()(Engine& engine, EntID me, AIComponent& myAi) {
        auto& vel = me.get<Velocity>().data();
        auto colinfo = me.get<CollisionData>();
        hitLeftRight = false;
        onFloor = false;
        shouldJump = false;

        if (std::rand() % 10 < 2)
            shouldJump = true;

        if (colinfo) {
            auto& coldata = colinfo.data();

            for (auto& hit : coldata.hits) {
                if (hit.dir == CollisionData::HitDir::RIGHT || hit.dir == CollisionData::HitDir::LEFT)
                    hitLeftRight = true;
                if (hit.dir == CollisionData::HitDir::DOWN || hit.eid.get<Solid>())
                    onFloor = true;
            } // end for
        } // end if

        if (hitLeftRight)
            movingRight = !movingRight;
        if (movingRight && vel.vel.x < maxVel)
            vel.acc.x += 500;
        if (!movingRight && -vel.vel.x < maxVel)
            vel.acc.x -= 500;
        if (!onFloor && vel.vel.y < maxVel)
            vel.acc.y += 4000;
        if (vel.vel.y == 0 && shouldJump)
            vel.acc.y -= 40000;
    } // end operator()

}; // end GoombaAI

#endif // LUDUMDARE32_AICOMPONENT_HPP
