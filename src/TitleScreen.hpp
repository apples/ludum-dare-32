//
// Created by Jeramy on 4/18/2015.
//

#ifndef LUDUMDARE32_TITLESCREEN_H
#define LUDUMDARE32_TITLESCREEN_H

#include "GameState.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <utility>
#include <memory>

class TitleScreen : public GameState {
    sf::Font font;
    sf::Text text;
    std::vector<std::pair<std::string, std::shared_ptr<GameState>>> items;
    int selected;
public:
    TitleScreen();
    virtual bool halts_update() const override;
    virtual bool halts_draw() const override;
    virtual void update(Engine& engine, double time_step) override;
    virtual void draw(sf::RenderWindow& window) const override;
};


#endif //LUDUMDARE32_TITLESCREEN_H
