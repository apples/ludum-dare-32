//
// Created by xxAtrain223 on 4/20/2015.
//

#ifndef LUDUMDARE32_SOUNDPLAYGROUND_HPP
#define LUDUMDARE32_SOUNDPLAYGROUND_HPP

#include "GameState.hpp"
#include "Engine.hpp"
#include "MusicComponent.hpp"
#include <sfml/Graphics.hpp>
#include <json/json.h>

class SoundPlayground : public GameState {

public:
    SoundPlayground();
    virtual bool halts_update() const override;
    virtual bool halts_draw() const override;
    virtual void update(Engine& engine, double time_step) override;
    virtual void draw(sf::RenderWindow& window) const override;
};

#endif //LUDUMDARE32_SOUNDPLAYGROUND_HPP
