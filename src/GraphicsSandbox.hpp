//
// Created by xxAtrain223 on 4/18/2015.
//

#ifndef LUDUMDARE32_GRAPHICSSANDBOX_H
#define LUDUMDARE32_GRAPHICSSANDBOX_H

#include "GameState.hpp"
#include "entcom.hpp"
#include "Camera.hpp"

#include <SFML/Graphics.hpp>


class GraphicsSandbox : public GameState {
    sf::Texture tileTex;
    DB mutable db;
    Camera cam;

public:
    GraphicsSandbox();
    virtual bool halts_update() const override;
    virtual bool halts_draw() const override;
    virtual void update(Engine& engine, double time_step) override;
    virtual void draw(sf::RenderWindow& window) const override;
};


#endif //LUDUMDARE32_GRAPHICSSANDBOX_H
