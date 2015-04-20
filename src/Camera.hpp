//
// Created by xxAtrain223 on 4/18/2015.
//

#ifndef LUDUMDARE32_CAMERA_HPP
#define LUDUMDARE32_CAMERA_HPP

#include <SFML/Graphics.hpp>
#include <ginseng/ginseng.hpp>
#include "entcom.hpp"
#include "components.hpp"
#include "Engine.hpp"
#include <vector>
#include <tuple>
#include <algorithm>

class Camera {
private:
    sf::FloatRect rect;
    sf::FloatRect moveableArea;

public:

    sf::Vector2f getPosition() const { return sf::Vector2f(rect.left, rect.top); }
    void setPosition(sf::Vector2f pos) {
        rect.left = pos.x; rect.top = pos.y;

        if (rect.width < moveableArea.width && rect.height < moveableArea.height) {
            if (rect.left < moveableArea.left){
                rect.left = moveableArea.left;
            }
            else if ((rect.left + rect.width) > (moveableArea.left + moveableArea.width)){
                rect.left = moveableArea.left + moveableArea.width - rect.width;
            }

            if (rect.top < moveableArea.top){
                rect.top = moveableArea.top;
            }
            else if ((rect.top + rect.height) > (moveableArea.top + moveableArea.height)){
                rect.top = moveableArea.top + moveableArea.height - rect.height;
            }
        }
    }
    void setPosition(float x, float y) { setPosition(sf::Vector2f(x, y)); }

    void move(sf::Vector2f offset) { setPosition(getPosition() + offset); }
    void move(float x, float y) { move(sf::Vector2f(x, y)); }

    sf::Vector2f getSize() const { return sf::Vector2f(rect.width, rect.height); }
    void setSize(sf::Vector2f size) { rect.width = size.x; rect.height = size.y; }
    void setSize(float width, float height) { rect.width = width; rect.height = height; }

    sf::FloatRect getMoveableArea() const { return moveableArea; }
    void setMoveableArea(sf::FloatRect area) { moveableArea = area; }

    void draw(sf::RenderWindow &window, DB& db) const {
        auto items = db.query<components::Sprite, components::BoundingBox>();
        auto spriteComp = [](const auto& ent1, const auto& ent2) {
            const auto& spr1 = std::get<1>(ent1).data();
            const auto& spr2 = std::get<1>(ent2).data();
            auto tup1 = std::make_tuple(spr1.layer, spr1.spr.getTexture());
            auto tup2 = std::make_tuple(spr2.layer, spr2.spr.getTexture());
            return tup1 < tup2;
        };
        std::sort(std::begin(items), std::end(items), spriteComp);

        for (auto& ent : items) {
            sf::FloatRect sprRect(std::get<2>(ent).data().rect);
            if (rect.intersects(sprRect)) {
                Animation spr(std::get<1>(ent).data().spr);
                spr.setPosition(-getPosition() + sf::Vector2f(sprRect.left, sprRect.top));
                window.draw(spr);
            }
        }
    }
};

#endif //LUDUMDARE32_CAMERA_HPP
