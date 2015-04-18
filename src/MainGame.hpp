//
// Created by Jeramy on 4/18/2015.
//

#ifndef LUDUMDARE32_MAINGAME_H
#define LUDUMDARE32_MAINGAME_H

#include "GameState.hpp"
#include "entcom.hpp"

#include <string>

class MainGame : public GameState {
    mutable DB entities;
public:
    MainGame();
    void load_level(std::string fname);

    virtual bool halts_update() const override;
    virtual bool halts_draw() const override;
    virtual void update(Engine &engine, double time_step) override;
    virtual void draw(sf::RenderWindow &window) const override;
};


#endif //LUDUMDARE32_MAINGAME_H
