//
// Created by Jeramy on 4/20/2015.
//

#ifndef LUDUMDARE32_UPDATESPRITES_HPP
#define LUDUMDARE32_UPDATESPRITES_HPP

inline void update_sprites(DB& db, double time_step) {
    using std::get;
    using components::Sprite;
    for (auto ent : db.query<Sprite>()) {
        auto& spr = get<1>(ent).data().spr;
        spr.update(sf::seconds(time_step));
    }
}

#endif //LUDUMDARE32_UPDATESPRITES_HPP
