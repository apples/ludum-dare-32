//
// Created by Jeramy on 4/19/2015.
//

#include "PhysicsEngine.hpp"

void physics_step(DB& db, double timeStep) {
    using std::get;
    using components::Velocity;
    using components::BoundingBox;
    using components::Solid;

    auto bb_vels = db.query<BoundingBox, Velocity>();
    auto bbs = db.query<BoundingBox>();

    for (auto ent : db.query<CollisionData>()) {
        db.eraseComponent(get<1>(ent).id());
    }

    for (auto ent_v : bb_vels) //search entities with velocities
    {
        EntID v_eid = get<0>(ent_v);
        BoundingBox &v_bb = get<1>(ent_v).data();
        Velocity &v_vel = get<2>(ent_v).data();

        v_bb.rect.left += v_vel.vel.x * timeStep;
        v_bb.rect.top += v_vel.vel.y * timeStep;

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
            if (v_bb.rect.intersects(bb.rect, isect))
            {
                if (bb_eid.get<Solid>()) {
                    if (isect.width > isect.height) {
                        if (v_bb.rect.top < bb.rect.top) {
                            v_bb.rect.top -= isect.height;
                        } else {
                            v_bb.rect.top += isect.height;
                        }
                        v_vel.vel.y = 0;
                        v_vel.vel.x *= 0.95;
                    } else {
                        if (v_bb.rect.left < bb.rect.left) {
                            v_bb.rect.left -= isect.width;
                        } else {
                            v_bb.rect.left += isect.width;
                        }
                        v_vel.vel.x = 0;
                        v_vel.vel.y *= 0.95;
                    }
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
    }
}
