//
// Created by Jeramy on 4/18/2015.
//

#ifndef LUDUMDARE32_TITLESCREEN_H
#define LUDUMDARE32_TITLESCREEN_H

#include "GameState.hpp"

#include <SFML/Graphics.hpp>

class TitleScreen : public GameState {
    sf::Font font;
    sf::Text text;
public:
    TitleScreen();
    virtual bool halts_update() const override;
    virtual bool halts_draw() const override;
    virtual void update(Engine& engine) override;
    virtual void draw(sf::RenderWindow& window) const override;
};


#endif //LUDUMDARE32_TITLESCREEN_H
