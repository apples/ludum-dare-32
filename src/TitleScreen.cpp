//
// Created by Jeramy on 4/18/2015.
//

#include "TitleScreen.hpp"
#include "Engine.hpp"
#include "MainGame.hpp"
#include "LevelEditor.hpp"
#include "GraphicsSandbox.hpp"

namespace {

template <typename T, typename... Ts>
std::function<std::shared_ptr<GameState>()> maker(Ts... ts) {
    return [=]() -> std::shared_ptr<GameState> {
        return std::make_shared<T>(ts...);
    };
}

} // static

TitleScreen::TitleScreen() {
    if (!font.loadFromFile("data/OFLGoudyStM.otf")) {
        throw std::runtime_error("Failed to load data/OFLGoudyStM.otf");
    }
    text.setFont(font);

    text.setString("Title Screen");
    text.setOrigin(text.getLocalBounds().width/2.f, text.getLocalBounds().height/2.f);

    items.emplace_back(std::make_pair("Main Game", maker<MainGame>()));
    items.emplace_back(std::make_pair("Level Editor", maker<LevelEditor>()));
    //items.emplace_back(std::make_pair("Graphics Sandbox", maker<GraphicsSandbox>()));

    selected = 0;
}

bool TitleScreen::halts_update() const {
    return true;
}

bool TitleScreen::halts_draw() const {
    return true;
}

void TitleScreen::update(Engine& engine, double time_step) {
    if (engine.wasKeyPressed(sf::Keyboard::Escape)) {
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
        engine.states.push(items[selected].second());
    }

    if (engine.wasMouseButtonPressed(sf::Mouse::Left))
    {
        sf::Text text(TitleScreen::text);
        for (int i = 0; i < items.size(); i++)
        {
            text.setString(items[i].first);
            text.setOrigin(text.getLocalBounds().width/2.f, 0);
            text.setPosition(engine.window.getSize().x/2, 100 + text.getLocalBounds().height * 2 * (i + 1));
            sf::FloatRect boundingBox;
            boundingBox = text.getGlobalBounds();
            ECHO(boundingBox.left, boundingBox.top, boundingBox.width, boundingBox.height);
            if (boundingBox.contains(engine.getMousePosition().x, engine.getMousePosition().y))
            {
                selected = i;
                engine.states.push(items[selected].second());
                break;
            }
        }
    }
}

void TitleScreen::draw(sf::RenderWindow& window) const {
    sf::Text text(TitleScreen::text);

    text.setOrigin(text.getLocalBounds().width/2.f, 0);
    text.setPosition(window.getSize().x / 2.f, 100);
    window.draw(text);

    for (int i = 0; i < items.size(); i++) {
        text.setString(items[i].first);
        text.setOrigin(text.getLocalBounds().width/2.f, 0);
        //text.setPosition(window.getSize().x / 2.f, 100 + text.getLocalBounds().height * 2 * (i + 1));
        text.setPosition(window.getSize().x / 2.f, 100 + text.getFont()->getLineSpacing(text.getCharacterSize()) * (i + 1));

        if (i == selected)
            text.setColor(sf::Color::Yellow);
        else
            text.setColor(sf::Color::White);

        window.draw(text);
    }
}
