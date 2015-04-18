//
// Created by Jeramy on 4/18/2015.
//

#include "TitleScreen.hpp"
#include "Engine.hpp"
#include "GraphicsSandbox.hpp"

TitleScreen::TitleScreen() {
    if (!font.loadFromFile("data/OFLGoudyStM.otf")) {
        throw std::runtime_error("Failed to load data/OFLGoudyStM.otf");
    }
    text.setFont(font);

    text.setString("Title Screen");
    text.setOrigin(text.getLocalBounds().width/2.f, text.getLocalBounds().height/2.f);

    items.emplace_back(std::make_pair("Graphics Sandbox", std::make_shared<GraphicsSandbox>()));

    selected = 0;
}

bool TitleScreen::halts_update() const {
    return true;
}

bool TitleScreen::halts_draw() const {
    return true;
}

void TitleScreen::update(Engine& engine, double time_step) {
    if (engine.isKeyDown(sf::Keyboard::Escape)) {
        engine.states.pop();
    }

    if (engine.isKeyDown(sf::Keyboard::R)) {
        text.setColor(sf::Color::Red);
    } else {
        text.setColor(sf::Color::White);
    }

    if (engine.wasKeyPressed(sf::Keyboard::W) || engine.wasKeyPressed(sf::Keyboard::Up)) {
        selected = (selected - 1 + items.size()) % items.size();
    }
    if (engine.wasKeyPressed(sf::Keyboard::S) || engine.wasKeyPressed(sf::Keyboard::Down)) {
        selected = (selected + 1 + items.size()) % items.size();
    }

    if (engine.wasKeyPressed(sf::Keyboard::Return) || engine.wasKeyPressed(sf::Keyboard::Space)) {
        engine.states.push(items[selected].second);
    }

    // TODO: Add mouse input
}

void TitleScreen::draw(sf::RenderWindow& window) const {
    sf::Text text(TitleScreen::text);

    text.setOrigin(text.getLocalBounds().width/2.f, 0);
    text.setPosition(window.getSize().x / 2.f, 100);
    window.draw(text);

    for (int i = 0; i < items.size(); i++) {
        text.setString(items[i].first);
        text.setOrigin(text.getLocalBounds().width/2.f, 0);
        text.setPosition(window.getSize().x / 2.f, 100 + text.getLocalBounds().height * 2 * (i + 1));

        if (i == selected)
            text.setColor(sf::Color::Yellow);
        else
            text.setColor(sf::Color::White);

        window.draw(text);
    }
}
