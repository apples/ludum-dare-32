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

PlayerAI::PlayerAI(std::function<EntID(EntID)> make_bear, Engine& engine, DB& db, EntID me) : make_bear(make_bear) {
    using namespace behavior;
    auto move = [&](auto key, auto dir, auto acc) {
        bool defer = true;
        return [&engine, me, key, dir, acc, defer]() mutable {
            if (engine.isKeyDown(key)) {
                if (auto collinfo = me.get<CollisionData>()) {
                    for (auto& hit : collinfo.data().hits) {
                        if (hit.dir == dir && hit.eid.get<Solid>()) {
                            defer = true;
                            return status::FAILURE;
                        }
                    }
                }
                me.get<Velocity>().data().acc.x += acc;
                if (defer) {
                    defer = false;
                    return status::RUNNING;
                } else {
                    return status::SUCCESS;
                }
            }
            defer = true;
            return status::FAILURE;
        };
    };

    auto set_anim = [&](auto anim_group, auto anim_name, bool flipped) {
        return [&db, me, anim_group, anim_name, flipped] {
            AiStateComponent state;
            state.anim_group = anim_group;
            state.anim_name = anim_name;
            state.flipped = flipped;
            db.makeComponent(me, state);
            return status::SUCCESS;
        };
    };

    auto move_left = move(sf::Keyboard::Left, CollisionData::HitDir::LEFT, -2000);
    auto move_right = move(sf::Keyboard::Right, CollisionData::HitDir::RIGHT, 2000);

    auto stand = [&db, me] {
        AiStateComponent state;
        state.anim_group = "player";
        state.anim_name = "stand";
        state.flipped = me.get<Sprite>().data().flipped;
        db.makeComponent(me, state);
        return status::SUCCESS;
    };

    std::weak_ptr<EntID> weak_bear;

    behaviors = sequence{
            selector{
                    sequence{
                            move_left,
                            set_anim("player", "walk", true),
                    },
                    sequence{
                            move_right,
                            set_anim("player", "walk", false),
                    },
                    stand,
            },
            never_fail{[&engine, me] {
                if (engine.wasKeyPressed(sf::Keyboard::Up)) {
                    if (auto collinfo = me.get<CollisionData>()) {
                        for (auto& hit : collinfo.data().hits) {
                            if (hit.dir == CollisionData::HitDir::DOWN && hit.eid.get<Solid>()) {
                                me.get<Velocity>().data().timed_accs.push_back({{0, -40000}, 0.015});
                                return status::SUCCESS;
                            }
                        }
                    }
                }
                return status::FAILURE;
            }},
            never_fail{when_running{
                    [&engine, &db, me, weak_bear, make_bear]() mutable {
                        if (auto bear_ptr = weak_bear.lock()) {
                            return status::RUNNING;
                        }
                        if (engine.wasKeyPressed(sf::Keyboard::Space)) {
                            auto bear = std::make_shared<EntID>(make_bear(me));
                            weak_bear = bear;
                            db.makeComponent(*bear, AIComponent{PlayerBearAI{me, make_bear}});
                            db.makeComponent(*bear, LockInput{});
                            db.makeComponent(*bear, [bear] { });
                            db.makeComponent(me, LookAt{*bear});
                            return status::RUNNING;
                        }
                        return status::FAILURE;
                    },
                    stand,
            }},
    };

    AiStateComponent state;
    state.anim_group = "player";
    state.anim_name = "stand";
    state.flipped = false;
    db.makeComponent(me, state);
}

void PlayerAI::operator()(Engine& engine, DB& db, EntID me, AIComponent& my_ai) {
    behaviors();
}

#if 0
void PlayerAI::operator()(Engine& engine, DB& db, EntID me, AIComponent& my_ai) {
    auto& vel = me.get<Velocity>().data();
    auto collinfo = me.get<CollisionData>();

    AiStateComponent state;
    state.flipped = me.get<Sprite>().data().flipped;
    state.anim_group = "player";
    state.anim_name = "stand";
    db.makeComponent(me, state);

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
        db.makeComponent(me, state);
    }
    if (engine.isKeyDown(sf::Keyboard::Right) || engine.isKeyDown(sf::Keyboard::D)) {
        vel.acc.x += 2000;
        state.anim_name = "walk";
        state.flipped = false;
        db.makeComponent(me, state);
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
        db.makeComponent(bear, AIComponent{PlayerBearAI{me, make_bear}});
        db.makeComponent(bear, LockInput{});
        db.makeComponent(me, LookAt{bear});
        db.makeComponent(me, AIComponent{PlayerAIIdle{}});
        return;
    }


}
#endif
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
        db.makeComponent(bear, AIComponent{PlayerBearAI{me, make_bear}});
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
    state.anim_group = "player";
    if (auto asc = me.get<AiStateComponent>()) {
        state.flipped = me.get<Sprite>().data().flipped;
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
    state.anim_group = "bear";
    bool attacking = bool(me.get<AttackTimer>());

    if (!attacking) {
        state.flipped = me.get<Sprite>().data().flipped;
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
        db.makeComponent(player, AIComponent{PlayerAIWithBear{me, make_bear}});
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
        //db.makeComponent(player, AIComponent{PlayerAI{make_bear,engine,db,player}});
        db.makeComponent(player, LockInput{});
        if (auto la = player.get<LookAt>()) {
            db.eraseComponent(la.id());
        }
        db.eraseEntity(me);
        return;
    }
    if (engine.wasKeyPressed(sf::Keyboard::Space)) {
        auto slash = db.makeEntity();
        db.makeComponent(slash,
                         BoundingBox{{bb.rect.left + (anim.flipped ? -1 : 1) * bb.rect.width, bb.rect.top, 16, 32}});
        db.makeComponent(slash, PainBox{PainBox::Team::PLAYER});
        db.makeComponent(me, AttackTimer{0.10});
        db.makeComponent(slash, TimerComponent{[&db, slash] { db.eraseEntity(slash); }, 0.10});
        state.anim_name = "attack";
        db.makeComponent(me, state);
    }
}
