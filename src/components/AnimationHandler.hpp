//
// Created by xxAtrain223 on 4/20/2015.
//

#ifndef LUDUMDARE32_ANIMATIONHANDLER_HPP
#define LUDUMDARE32_ANIMATIONHANDLER_HPP

#include "Engine.hpp"
#include "components.hpp"
#include "entcom.hpp"

#include <sfml/Graphics.hpp>
#include <ginseng/ginseng.hpp>

void updateAnimations(DB db) {
    using namespace components;
    auto items = db.query<Sprite, BoundingBox, Velocity>();

    for (auto& ent : items) {
        Sprite spr = std::get<1>(ent).data();
        BoundingBox bb = std::get<2>(ent).data();
        Velocity vel = std::get<3>(ent).data();

        if (vel.vel.x < 0) {
            spr.flipped = true;
        }
        else if (vel.vel.x > 0) {
            spr.flipped = false;
        }
    }
}

#endif //LUDUMDARE32_ANIMATIONHANDLER_HPP
