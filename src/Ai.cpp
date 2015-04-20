//
// created by Patrick Brukiewa - 18 Apr 15
//

#include "Ai.hpp"
#include "Engine.hpp"
#include "components.hpp"
#include "LoadAnimation.hpp"

using components::Solid;
using components::LookAt;
using components::LockInput;

void update_ais(Engine& engine, DB& db) {
    for (auto ent : db.query<AIComponent>()) {
        std::get<1>(ent).data().update(engine, db, std::get<0>(ent));
    }
}

void PlayerAI::operator()(Engine& engine, DB& db, EntID me, AIComponent& my_ai) {
    auto& vel = me.get<Velocity>().data();
    auto collinfo = me.get<CollisionData>();

    {
        auto li = me.get<LockInput>();
        if (li) {
            db.eraseComponent(li.id());
            return;
        }
    }

    if (engine.isKeyDown(sf::Keyboard::Left) || engine.isKeyDown(sf::Keyboard::A)) {
        vel.acc.x += -2000;
    }
    if (engine.isKeyDown(sf::Keyboard::Right) || engine.isKeyDown(sf::Keyboard::D)) {
        vel.acc.x += 2000;
    }
    if (engine.wasKeyPressed(sf::Keyboard::Up) || engine.wasKeyPressed(sf::Keyboard::W)) {
        if (collinfo) {
            for (auto& hit : collinfo.data().hits) {
                if (hit.dir == CollisionData::HitDir::DOWN && hit.eid.get<Solid>()) {
                    vel.timed_accs.push_back({{0, -40000}, 0.015});
                    break;
                }
            }
        }
    }
    if (engine.wasKeyPressed(sf::Keyboard::Space)) {
        auto bear = make_bear(me);
        db.makeComponent(bear, AIComponent{PlayerBearAI{me,make_bear}});
        db.makeComponent(bear, LockInput{});
        db.makeComponent(me, LookAt{bear});
        db.makeComponent(me, AIComponent{PlayerAIIdle{}});
        return;
    }
}

void PlayerAIWithBear::operator()(Engine& engine, DB& db, EntID me, AIComponent& my_ai) {
    auto& vel = me.get<Velocity>().data();
    auto collinfo = me.get<CollisionData>();

    {
        auto li = me.get<LockInput>();
        if (li) {
            db.eraseComponent(li.id());
            return;
        }
    }

    if (engine.isKeyDown(sf::Keyboard::Left) || engine.isKeyDown(sf::Keyboard::A)) {
        vel.acc.x += -2000;
    }
    if (engine.isKeyDown(sf::Keyboard::Right) || engine.isKeyDown(sf::Keyboard::D)) {
        vel.acc.x += 2000;
    }
    if (engine.wasKeyPressed(sf::Keyboard::Up) || engine.wasKeyPressed(sf::Keyboard::W)) {
        if (collinfo) {
            for (auto& hit : collinfo.data().hits) {
                if (hit.dir == CollisionData::HitDir::DOWN && hit.eid.get<Solid>()) {
                    vel.timed_accs.push_back({{0, -40000}, 0.015});
                    break;
                }
            }
        }
    }

    if (engine.wasKeyPressed(sf::Keyboard::Tab) || engine.wasKeyPressed(sf::Keyboard::Space)) {
        db.makeComponent(bear, AIComponent{PlayerBearAI{me,make_bear}});
        db.makeComponent(bear, LockInput{});
        db.makeComponent(me, LookAt{bear});
        db.makeComponent(me, AIComponent{PlayerAIIdle{}});
        return;
    }
}

void PlayerAIIdle::operator()(Engine& engine, DB& db, EntID me, AIComponent& my_ai) {
    auto& vel = me.get<Velocity>().data();
    auto collinfo = me.get<CollisionData>();
}

void PlayerBearAI::operator()(Engine& engine, DB& db, EntID me, AIComponent& my_ai) {
    auto& vel = me.get<Velocity>().data();
    auto collinfo = me.get<CollisionData>();

    {
        auto li = me.get<LockInput>();
        if (li) {
            db.eraseComponent(li.id());
            return;
        }
    }

    if (engine.isKeyDown(sf::Keyboard::Left) || engine.isKeyDown(sf::Keyboard::A)) {
        vel.acc.x += -2000;
    }
    if (engine.isKeyDown(sf::Keyboard::Right) || engine.isKeyDown(sf::Keyboard::D)) {
        vel.acc.x += 2000;
    }
    if (engine.wasKeyPressed(sf::Keyboard::Up) || engine.wasKeyPressed(sf::Keyboard::W)) {
        if (collinfo) {
            for (auto& hit : collinfo.data().hits) {
                if (hit.dir == CollisionData::HitDir::DOWN && hit.eid.get<Solid>()) {
                    vel.timed_accs.push_back({{0, -40000}, 0.015});
                    break;
                }
            }
        }
    }
    if (engine.wasKeyPressed(sf::Keyboard::Tab)) {
        db.makeComponent(player, AIComponent{PlayerAIWithBear{me,make_bear}});
        db.makeComponent(player, LockInput{});
        {
            auto la = player.get<LookAt>();
            if (la) {
                db.eraseComponent(la.id());
            }
        }
        db.makeComponent(me, AIComponent{BearAI{}}); // TODO: BearAI
        return;
    }
    if (engine.wasKeyPressed(sf::Keyboard::Return)) {
        db.makeComponent(player, AIComponent{PlayerAI{make_bear}});
        db.makeComponent(player, LockInput{});
        {
            auto la = player.get<LookAt>();
            if (la) {
                db.eraseComponent(la.id());
            }
        }
        db.eraseEntity(me);
        return;
    }
    if (engine.wasKeyPressed(sf::Keyboard::Space)) {
        //TODO: Attack
    }
}
