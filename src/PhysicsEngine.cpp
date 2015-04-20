//
// Created by Jeramy on 4/19/2015.
//

#include "PhysicsEngine.hpp"
#include "echo.hpp"

namespace {

using HitDirPair = std::pair<CollisionData::HitDir,CollisionData::HitDir>;

struct GetX {
    template<typename T>
    auto operator()(T &t) -> decltype((t.x)) { return t.x; }
    auto operator()(sf::FloatRect &t) -> decltype((t.left)) { return t.left; }
    auto len(sf::FloatRect &t) -> decltype((t.width)) { return t.width; }
    template<typename T>
    auto other(T &t) -> decltype((t.y)) { return t.y; }
    HitDirPair hitdirs(const sf::FloatRect& e1, const sf::FloatRect& e2) {
        HitDirPair rv = {CollisionData::HitDir::RIGHT,CollisionData::HitDir::LEFT};
        if (e1.left < e2.left) {
        } else {
            std::swap(rv.first,rv.second);
        }
        return rv;
    }
};

struct GetY {
    template<typename T>
    auto operator()(T &t) -> decltype((t.y)) { return t.y; }
    auto operator()(sf::FloatRect &t) -> decltype((t.top)) { return t.top; }
    auto len(sf::FloatRect &t) -> decltype((t.height)) { return t.height; }
    template<typename T>
    auto other(T &t) -> decltype((t.x)) { return t.x; }
    HitDirPair hitdirs(const sf::FloatRect& e1, const sf::FloatRect& e2) {
        HitDirPair rv = {CollisionData::HitDir::DOWN,CollisionData::HitDir::UP};
        if (e1.top < e2.top) {
        } else {
            std::swap(rv.first,rv.second);
        }
        return rv;
    }
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
        for (auto& p : v_vel.timed_accs) {
            double t = std::min(p.second,timeStep);
            p.second -= t;
            v_vel.acc += p.first * float(t/timeStep);
        }
        v_vel.timed_accs.erase(std::remove_if(v_vel.timed_accs.begin(), v_vel.timed_accs.end(), [](auto& p){return p.second<=0;}), v_vel.timed_accs.end());
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

                    auto hitdirs = get_axis.hitdirs(v_bb.rect, bb.rect);
                    v_coll_info.data().hits.push_back(CollisionData::Hit{bb_eid,hitdirs.first});
                    bb_coll_info.data().hits.push_back(CollisionData::Hit{v_eid,hitdirs.second});
                }
            }
            get_axis(v_vel.acc) = 0;
        }
    };

    do_axis(GetX{});
    do_axis(GetY{});
}
