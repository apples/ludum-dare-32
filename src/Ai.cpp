//
// created by Patrick Brukiewa - 18 Apr 15
//

#include "Ai.hpp"
#include "Engine.hpp"
#include "components.hpp"
#include "LoadAnimation.hpp"

using namespace components;

void update_ais(Engine& engine, DB& db) {
    for (auto ent : db.query<AIComponent>()) {
        std::get<1>(ent).data().update(engine, db, std::get<0>(ent));
    }
}

void PlayerAI::operator()(Engine& engine, DB& db, EntID me, AIComponent& my_ai) {
    auto& vel = me.get<Velocity>().data();
    auto collinfo = me.get<CollisionData>();

    AiStateComponent state;
    state.flipped = me.get<Sprite>().data().flipped;
    state.anim_group = "player";
    state.anim_name = "stand";
    db.makeComponent(me,state);

    {
        auto li = me.get<LockInput>();
        if (li) {
            db.eraseComponent(li.id());
            return;
        }
    }

    if (engine.isKeyDown(sf::Keyboard::Left) || engine.isKeyDown(sf::Keyboard::A)) {
        vel.acc.x += -2000;
        state.anim_name = "walk";
        state.flipped = true;
        db.makeComponent(me,state);
    }
    if (engine.isKeyDown(sf::Keyboard::Right) || engine.isKeyDown(sf::Keyboard::D)) {
        vel.acc.x += 2000;
        state.anim_name = "walk";
        state.flipped = false;
        db.makeComponent(me,state);
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
    AiStateComponent state;
    state.flipped = me.get<Sprite>().data().flipped;
    state.anim_group = "player";
    state.anim_name = "stand";

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
    auto& anim = me.get<Sprite>().data();
    auto collinfo = me.get<CollisionData>();
    AiStateComponent state;
    if (auto asc = me.get<AiStateComponent>()) {
        state.flipped = me.get<Sprite>().data().flipped;
        state.anim_group = "player";
        state.anim_name = "stand";
        db.makeComponent(me, state);
    }
}

void PlayerBearAI::operator()(Engine& engine, DB& db, EntID me, AIComponent& my_ai) {
    auto& vel = me.get<Velocity>().data();
    auto& bb = me.get<BoundingBox>().data();
    auto& anim = me.get<Sprite>().data();
    auto collinfo = me.get<CollisionData>();

    AiStateComponent state;
    bool attacking = false;

    if (auto asc = me.get<AiStateComponent>()) {
        if ((asc.data().anim_name=="hurt" || asc.data().anim_name=="attack")) {
            attacking = true;
        }
    }
    if (!attacking) {
        state.flipped = me.get<Sprite>().data().flipped;
        state.anim_group = "bear";
        state.anim_name = "stand";
        db.makeComponent(me, state);
    }

    {
        auto li = me.get<LockInput>();
        if (li) {
            db.eraseComponent(li.id());
            return;
        }
    }

    if (engine.isKeyDown(sf::Keyboard::Left) || engine.isKeyDown(sf::Keyboard::A)) {
        vel.acc.x += -2000;
        if (!attacking) {
            state.anim_name = "walk";
            state.flipped = true;
            db.makeComponent(me, state);
        }
    }
    if (engine.isKeyDown(sf::Keyboard::Right) || engine.isKeyDown(sf::Keyboard::D)) {
        vel.acc.x += 2000;
        if (!attacking) {
            state.anim_name = "walk";
            state.flipped = false;
            db.makeComponent(me, state);
        }
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
        db.makeComponent(me, AIComponent{BearAI{}});
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
        auto slash = db.makeEntity();
        db.makeComponent(slash, BoundingBox{{bb.rect.left+(anim.flipped?-1:1)*bb.rect.width,bb.rect.top,16,32}});
        db.makeComponent(slash, PainBox{PainBox::Team::PLAYER});
        db.makeComponent(slash, TimerComponent{[&db,slash,me]{db.eraseEntity(slash);if (auto asc = me.get<AiStateComponent>()) {db.eraseComponent(asc.id());}},0.10});
        state.anim_name = "attack";
        db.makeComponent(me,state);
    }
}
