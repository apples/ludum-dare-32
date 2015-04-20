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

using namespace components;

class Engine;

void update_ais(Engine& engine, DB& db);

struct AIComponent {
    template <class T> AIComponent(T inputAi) { brain = inputAi; }
    std::function<void(Engine&, DB&, EntID, AIComponent &)> brain;

    void update(Engine& engine, DB& db, EntID id) { brain(engine, db, id, *this); }

};

struct AiStateComponent {
    std::string anim_name;
    std::string anim_group;
    bool flipped;

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
    bool hitWall;
    bool onFloor;
    bool shouldJump;
    double maxVel = 2000;

    void operator()(Engine& engine, DB& db, EntID me, AIComponent& myAi) {
        auto& vel = me.get<Velocity>().data();
        auto colinfo = me.get<CollisionData>();
        std::unique_ptr<EntID> target;
        double distance = 500;
        double tmpDist;
        double myX, myY, enemyX, enemyY, targetX, targetY;
        auto& bb = me.get<BoundingBox>().data();
        auto& anim = me.get<Sprite>().data();
        double width = bb.rect.width;

        hitWall = false;
        onFloor = false;

        auto enemies = db.query<Enemy>();
        
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
                if ((hit.dir == CollisionData::HitDir::RIGHT
                        || hit.dir == CollisionData::HitDir::LEFT)
                        && hit.eid.get<Solid>())
                    hitWall= true;
                if (hit.dir == CollisionData::HitDir::DOWN
                        && hit.eid.get<Solid>())
                    onFloor = true;
            } // end for
        } // end if

        if (target) {
            if (hitWall && onFloor)
                vel.timed_accs.push_back({{0, -60000}, 0.015});
            if (targetX < myX + width && -vel.vel.x < maxVel)
                vel.acc.x -= 1000;
            if (targetX > myX - width && vel.vel.x < maxVel)
                vel.acc.x += 1000;
            if (distance < 16) {
                auto slash = db.makeEntity();
                db.makeComponent(slash, BoundingBox{{bb.rect.left+(anim.flipped?-1:1)*bb.rect.width,bb.rect.top, 16, 32}});
                db.makeComponent(slash, PainBox{PainBox::Team::PLAYER});
                db.makeComponent(slash, TimerComponent{[&db, slash]{db.eraseEntity(slash);},0.10});
            } // end if
        } // end if

        else {
        } // end else
                
    } // end operator()
}; // end BearAI
    

struct GoombaAI {
    bool movingRight = true;
    bool hitWall;
    bool onFloor;
    bool shouldJump;
    double maxVel = 2000;

    void operator()(Engine& engine, DB& db, EntID me, AIComponent& myAi) {
        auto& vel = me.get<Velocity>().data();
        auto colinfo = me.get<CollisionData>();
        hitWall = false;
        onFloor = false;

        if (colinfo) {
            auto& coldata = colinfo.data();

            for (auto& hit : coldata.hits) {
                if (auto pb = hit.eid.get<PainBox>()) {
                    if (pb.data().team == PainBox::Team::PLAYER) {
                        //TODO: smokebomb
                        db.eraseEntity(me);
                        return;
                    }
                }
                if ((hit.dir == CollisionData::HitDir::RIGHT
                        || hit.dir == CollisionData::HitDir::LEFT)
                        && hit.eid.get<Solid>())
                    hitWall = true;
                if (hit.dir == CollisionData::HitDir::DOWN || hit.eid.get<Solid>())
                    onFloor = true;
            } // end for
        } // end if

        if (hitWall)
            movingRight = !movingRight;
        if (movingRight && vel.vel.x < maxVel)
            vel.acc.x += 500;
        if (!movingRight && -vel.vel.x < maxVel)
            vel.acc.x -= 500;
        if (!onFloor && vel.vel.y < maxVel)
            vel.acc.y += 4000;
    } // end operator()

}; // end GoombaAI

#endif // LUDUMDARE32_AICOMPONENT_HPP
