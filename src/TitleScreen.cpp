//
// Created by Jeramy on 4/18/2015.
//

#include "TitleScreen.hpp"
#include "Engine.hpp"

TitleScreen::TitleScreen() {
    if (!font.loadFromFile("data/OFLGoudyStM.otf")) {
        throw std::runtime_error("Failed to load data/OFLGoudyStM.otf");
    }
    text.setFont(font);
    text.setString("Title Screen");
    text.setOrigin(text.getLocalBounds().width/2.0,text.getLocalBounds().height/2.0);
    text.setPosition(400,300);
}

bool TitleScreen::halts_update() const {
    return true;
}

bool TitleScreen::halts_draw() const {
    return true;
}

void TitleScreen::update(Engine& engine) {
    if (engine.isKeyDown(sf::Keyboard::S)) {
        text.setColor(sf::Color::Red);
    } else {
        text.setColor(sf::Color::White);
    }
}

void TitleScreen::draw(sf::RenderWindow& window) const {
    window.draw(text);
}
