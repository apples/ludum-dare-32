//
// Created by Jeramy on 4/19/2015.
//

#include "PhysicsEngine.hpp"

namespace {

struct GetX {
    template<typename T>
    auto operator()(T &t) -> decltype((t.x)) { return t.x; }
    auto operator()(sf::FloatRect &t) -> decltype((t.left)) { return t.left; }
    auto len(sf::FloatRect &t) -> decltype((t.width)) { return t.width; }
    template<typename T>
    auto other(T &t) -> decltype((t.y)) { return t.y; }
};

struct GetY {
    template<typename T>
    auto operator()(T &t) -> decltype((t.y)) { return t.y; }
    auto operator()(sf::FloatRect &t) -> decltype((t.top)) { return t.top; }
    auto len(sf::FloatRect &t) -> decltype((t.height)) { return t.height; }
    template<typename T>
    auto other(T &t) -> decltype((t.x)) { return t.x; }
};

} // static

void physics_step(DB& db, double timeStep) {
    using std::get;
    using components::Velocity;
    using components::BoundingBox;
    using components::Solid;

    auto gravity = 1500.0;

    auto bb_vels = db.query<BoundingBox, Velocity>();
    auto bbs = db.query<BoundingBox>();

    for (auto ent : db.query<CollisionData>()) {
        db.eraseComponent(get<1>(ent).id());
    }

    for (auto ent_v : bb_vels) {
        Velocity &v_vel = get<2>(ent_v).data();
        v_vel.acc.y += gravity;
    }

    auto do_axis = [&](auto get_axis) {
        for (auto ent_v : bb_vels) //search entities with velocities
        {
            EntID v_eid = get<0>(ent_v);
            BoundingBox &v_bb = get<1>(ent_v).data();
            Velocity &v_vel = get<2>(ent_v).data();

            get_axis(v_vel.vel) += get_axis(v_vel.acc) * timeStep;

            get_axis(v_bb.rect) += get_axis(v_vel.vel) * timeStep + 0.5 * get_axis(v_vel.acc) * timeStep * timeStep;

            auto v_coll_info = v_eid.get<CollisionData>();
            if (!v_coll_info) {
                db.makeComponent(v_eid, CollisionData{});
                v_coll_info = v_eid.get<CollisionData>();
            }

            for (auto ent_bb : bbs)    //search entities with bounding boxes
            {
                EntID bb_eid = get<0>(ent_bb);
                BoundingBox &bb = get<1>(ent_bb).data();
                if (v_eid == bb_eid)
                    continue;

                sf::FloatRect isect;
                if (v_bb.rect.intersects(bb.rect, isect)) {
                    if (bb_eid.get<Solid>()) {
                        if (get_axis(v_bb.rect) < get_axis(bb.rect)) {
                            get_axis(v_bb.rect) -= get_axis.len(isect);
                        } else {
                            get_axis(v_bb.rect) += get_axis.len(isect);
                        }
                        get_axis(v_vel.vel) = 0;
                        get_axis.other(v_vel.vel) *= 0.95;
                    }

                    auto bb_coll_info = bb_eid.get<CollisionData>();
                    if (!bb_coll_info) {
                        db.makeComponent(bb_eid, CollisionData{});
                        bb_coll_info = bb_eid.get<CollisionData>();
                    }

                    v_coll_info.data().hits.push_back(bb_eid);
                    bb_coll_info.data().hits.push_back(v_eid);
                }
            }
            get_axis(v_vel.acc) = 0;
        }
    };

    do_axis(GetX{});
    do_axis(GetY{});
}
