//
// Created by Jeramy on 4/18/2015.
//

#include "TitleScreen.hpp"

TitleScreen::TitleScreen() {
    if (!font.loadFromFile("data/OFLGoudyStM.otf")) {
        throw std::runtime_error("Failed to load data/OFLGoudyStM.otf");
    }
}

bool TitleScreen::halts_update() const {
    return true;
}

bool TitleScreen::halts_draw() const {
    return true;
}

void TitleScreen::update() {

}

void TitleScreen::draw(sf::RenderWindow& window) const {
    sf::Text text;
    text.setFont(font);
    text.setString("Title Screen");
    text.setColor(sf::Color::White);
    text.setOrigin(text.getLocalBounds().width/2.0,text.getLocalBounds().height/2.0);
    text.setPosition(400,300);
    window.draw(text);
}
