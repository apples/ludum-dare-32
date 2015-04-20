//
// Created by Jeramy on 4/19/2015.
//

#include "LevelEditor.hpp"
#include "echo.hpp"
#include "MainGame.hpp"

#include "Engine.hpp"

#include <fstream>
#include <string>

LevelEditorSave::LevelEditorSave(std::string prompt) : prompt(prompt) {
    font.loadFromFile("data/OFLGoudyStM.otf");
}

bool LevelEditorSave::halts_update() const {
    return true;
}

bool LevelEditorSave::halts_draw() const {
    return false;
}

void LevelEditorSave::update(Engine &engine, double time_step) {
    if (engine.wasKeyPressed(sf::Keyboard::Escape)) {
        fname = "";
        engine.states.pop();
    }

    for (auto event : engine.getKeyBuffer()) {
        if (event.code == sf::Keyboard::BackSpace && !fname.empty()) {
            fname.pop_back();
        } else if (event.code >= sf::Keyboard::A && event.code <= sf::Keyboard::Z) {
            char c = (event.shift ? 'A' : 'a');
            c += (event.code - sf::Keyboard::A);
            fname.push_back(c);
        }
    }

    if (engine.wasKeyPressed(sf::Keyboard::Return)) {
        engine.states.pop();
    }
}

void LevelEditorSave::draw(sf::RenderWindow &window) const {
    sf::Text text(prompt + ": [ " + fname + " ]", font);
    auto tb = text.getLocalBounds();
    text.setOrigin(tb.width / 2.0, tb.height / 2.0);
    auto ws = window.getSize();
    text.setPosition(ws.x / 2.0, ws.y / 2.0);
    window.draw(text);
}

LevelEditorPick::LevelEditorPick(int* active_tile, const sf::Texture* tex) : active_tile(active_tile), tex(tex) {}

bool LevelEditorPick::halts_update() const {
    return true;
}

bool LevelEditorPick::halts_draw() const {
    return false;
}

void LevelEditorPick::update(Engine& engine, double time_step) {
    if (engine.wasMouseButtonPressed(sf::Mouse::Left)) {
        auto tsz = sf::Vector2f(tex->getSize());
        auto sprW = tsz.x / 16;
        auto sprH = tsz.y / 16;
        auto texorig = sf::Vector2f(400,300)-tsz/2.f;
        int tx = (engine.getMousePosition().x - texorig.x) / sprW;
        int ty = (engine.getMousePosition().y - texorig.y) / sprH;
        if (tx >= 0 && tx < 16 && ty >= 0 && ty < 16) {
            *active_tile = ty*16+tx;
            return engine.states.pop();
        }
    }

    if (engine.wasKeyPressed(sf::Keyboard::Escape) || engine.wasKeyPressed(sf::Keyboard::Space)) {
        return engine.states.pop();
    }
}

void LevelEditorPick::draw(sf::RenderWindow& window) const {
    sf::RectangleShape rect;
    sf::Sprite spr (*tex);
    auto texsz = sf::Vector2f(tex->getSize());
    spr.setPosition(sf::Vector2f(400,300)-texsz/2.f);
    rect.setSize(texsz*1.125f);
    rect.setPosition(sf::Vector2f(400,300)-texsz*1.125f/2.f);
    rect.setFillColor(sf::Color(0,0,0,128));
    window.draw(rect);
    window.draw(spr);
}

LevelEditor::LevelEditor() {
    font.loadFromFile("data/OFLGoudyStM.otf");
    json["width"] = 10;
    json["height"] = 10;
    json["rows"].resize(json["height"].asInt());
    for (auto i = 0; i < json["height"].asInt(); ++i) {
        json["rows"][i].resize(json["width"].asInt());
        for (auto j = 0; j < json["width"].asInt(); ++j) {
            json["rows"][i][j] = (i + j) % 2;
        }
    }
    //ECHO(json);
}

bool LevelEditor::halts_update() const {
    return true;
}

bool LevelEditor::halts_draw() const {
    return true;
}

void LevelEditor::update(Engine &engine, double time_step) {
    if (saver) {
        if (!saver->fname.empty()) {
            std::ofstream file("data/" + saver->fname + ".json");
            file << json;
            fname = saver->fname;
        }
        saver = {};
    }
    if (loader) {
        if (!loader->fname.empty()) {
            std::ifstream file("data/" + loader->fname + ".json");
            file >> json;
            fname = loader->fname;
        }
        loader = {};
    }

    if (engine.wasMouseButtonPressed(sf::Mouse::Left) ||
        (engine.isMouseButtonDown(sf::Mouse::Left) && engine.isKeyDown(sf::Keyboard::LShift))) {
        auto &tex = textures.get("terrain");
        auto tsz = tex.getSize();
        auto sprW = tsz.x / 16;
        auto sprH = tsz.y / 16;
        int tx = (engine.getMousePosition().x - camoffset.x) / sprW;
        int ty = (engine.getMousePosition().y - camoffset.y) / sprH;
        if (tx >= 0 && tx < json["width"].asInt() && ty >= 0 && ty < json["height"].asInt()) {
            json["rows"][ty][tx] = active_tile;
        }
    }

    if (engine.wasKeyPressed(sf::Keyboard::Numpad6)) {
        json["width"] = json["width"].asInt() + 1;
        for (auto r = 0; r < json["height"].asInt(); ++r) {
            json["rows"][r].append(active_tile);
        }
    }

    if (engine.wasKeyPressed(sf::Keyboard::Numpad4)) {
        json["width"] = json["width"].asInt() - 1;
        for (auto r = 0; r < json["height"].asInt(); ++r) {
            json["rows"][r].resize(json["width"].asInt());
        }
    }

    if (engine.wasKeyPressed(sf::Keyboard::Numpad2)) {
        json["height"] = json["height"].asInt() + 1;
        Json::Value a_row;
        for (auto i = 0; i < json["width"].asInt(); ++i) {
            a_row.append(active_tile);
        }
        json["rows"].append(a_row);
    }

    if (engine.wasKeyPressed(sf::Keyboard::Numpad8)) {
        json["height"] = json["height"].asInt() - 1;
        json["rows"].resize(json["height"].asInt());
    }

    if (engine.wasKeyPressed(sf::Keyboard::Add)) {
        ++active_tile;
    }

    if (engine.wasKeyPressed(sf::Keyboard::Subtract)) {
        --active_tile;
    }

    if (active_tile < 0) {
        active_tile = 0;
    }

    if (active_tile > 255) {
        active_tile = 255;
    }

    sf::Vector2f delta(0, 0);

    if (engine.isKeyDown(sf::Keyboard::W) || engine.isKeyDown(sf::Keyboard::Up)) {
        --delta.y;
    }
    if (engine.isKeyDown(sf::Keyboard::S) || engine.isKeyDown(sf::Keyboard::Down)) {
        ++delta.y;
    }
    if (engine.isKeyDown(sf::Keyboard::A) || engine.isKeyDown(sf::Keyboard::Left)) {
        --delta.x;
    }
    if (engine.isKeyDown(sf::Keyboard::D) || engine.isKeyDown(sf::Keyboard::Right)) {
        ++delta.x;
    }

    camoffset -= (delta * 1000.f * float(time_step));

    if (engine.wasKeyPressed(sf::Keyboard::F2)) {
        saver = std::make_shared<LevelEditorSave>("SAVE");
        saver->fname = fname;
        engine.states.push(saver);
    }

    if (engine.wasKeyPressed(sf::Keyboard::F3)) {
        loader = std::make_shared<LevelEditorSave>("LOAD");
        loader->fname = fname;
        engine.states.push(loader);
    }

    if (engine.wasKeyPressed(sf::Keyboard::F5)) {
        engine.states.push(std::make_shared<MainGame>(json));
    }

    if (engine.wasKeyPressed(sf::Keyboard::Space)) {
        engine.states.push(std::make_shared<LevelEditorPick>(&active_tile, &textures.get("terrain")));
    }

    if (engine.wasKeyPressed(sf::Keyboard::Escape)) {
        engine.states.pop();
    }
}

void LevelEditor::draw(sf::RenderWindow &window) const {
    sf::Sprite sprite;
    auto &tex = textures.get("terrain");
    auto tsz = tex.getSize();
    auto sprW = tsz.x / 16;
    auto sprH = tsz.y / 16;
    sprite.setTexture(tex);
    {
        sf::RectangleShape rect(sf::Vector2f(json["width"].asInt() * sprW, json["height"].asInt() * sprH));
        rect.setPosition(camoffset);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Magenta);
        rect.setOutlineThickness(5.0);
        window.draw(rect);
    }
    for (auto r = 0; r < json["height"].asInt(); ++r) {
        for (auto c = 0; c < json["width"].asInt(); ++c) {
            auto val = json["rows"][r][c].asInt();
            if (val > 0) {
                sprite.setPosition(c * sprW + camoffset.x, r * sprH + camoffset.y);
                auto quot = val / 16;
                auto rem = val % 16;
                sprite.setTextureRect(sf::IntRect(rem * sprW, quot * sprH, sprW, sprH));
                window.draw(sprite);
            }
        }
    }
    sf::Text text(std::to_string(active_tile), font);
    sf::RectangleShape rect(sf::Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height + 20));
    text.setPosition(0, 0);
    text.setColor(sf::Color::White);
    rect.setPosition(text.getPosition());
    rect.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(rect);
    window.draw(text);
}
