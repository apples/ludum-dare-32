//
// Created by Jeramy on 4/19/2015.
//

#include "LevelEditor.hpp"
#include "echo.hpp"

#include "Engine.hpp"

#include <fstream>
#include <string>

LevelEditorSave::LevelEditorSave() {
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
        engine.states.pop();
    }

    for (auto event : engine.getKeyBuffer()) {
        if (event.code == sf::Keyboard::BackSpace) {
            fname.pop_back();
        } else if (event.code >= sf::Keyboard::A && event.code <= sf::Keyboard::Z) {
            char c = (event.shift? 'A' : 'a');
            c += (event.code - sf::Keyboard::A);
            fname.push_back(c);
        }
    }

    if (engine.wasKeyPressed(sf::Keyboard::Return)) {
        engine.states.pop();
    }
}

void LevelEditorSave::draw(sf::RenderWindow &window) const {
    sf::Text text (fname, font);
    auto tb = text.getLocalBounds();
    text.setOrigin(tb.width/2.0, tb.height/2.0);
    auto ws = window.getSize();
    text.setPosition(ws.x/2.0, ws.y/2.0);
    window.draw(text);
}


LevelEditor::LevelEditor() {
    font.loadFromFile("data/OFLGoudyStM.otf");
    json["width"] = 10;
    json["height"] = 10;
    json["rows"].resize(json["height"].asInt());
    for (auto i = 0; i<json["height"].asInt(); ++i) {
        json["rows"][i].resize(json["width"].asInt());
        for (auto j = 0; j<json["width"].asInt(); ++j) {
            json["rows"][i][j] = (i+j)%2;
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
        std::ofstream file ("data/"+saver->fname+".json");
        file << json;
    }

    if (engine.wasMouseButtonPressed(sf::Mouse::Left)) {
        auto& tex = textures.get("terrain");
        auto tsz = tex.getSize();
        auto sprW = tsz.x/16;
        auto sprH = tsz.y/16;
        auto tx = engine.getMousePosition().x/sprW;
        auto ty = engine.getMousePosition().y/sprH;
        if (tx>=0 && tx<json["width"].asInt() && ty>=0 && ty<json["height"].asInt()) {
            json["rows"][ty][tx] = active_tile;
        }
    }

    if (engine.wasKeyPressed(sf::Keyboard::Numpad6)) {
        json["width"] = json["width"].asInt() + 1;
        for (auto r = 0; r<json["height"].asInt(); ++r) {
            json["rows"][r].append(active_tile);
        }
    }

    if (engine.wasKeyPressed(sf::Keyboard::Numpad2)) {
        json["height"] = json["height"].asInt() + 1;
        Json::Value a_row;
        for (auto i=0; i<json["width"].asInt(); ++i) {
            a_row.append(active_tile);
        }
        json["rows"].append(a_row);
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

    if (engine.wasKeyPressed(sf::Keyboard::F2)) {
        saver = std::make_shared<LevelEditorSave>();
        engine.states.push(saver);
    }

    if (engine.wasKeyPressed(sf::Keyboard::Escape)) {
        engine.states.pop();
    }
}

void LevelEditor::draw(sf::RenderWindow &window) const {
    sf::Sprite sprite;
    auto& tex = textures.get("terrain");
    auto tsz = tex.getSize();
    auto sprW = tsz.x/16;
    auto sprH = tsz.y/16;
    sprite.setTexture(tex);
    for (auto r = 0; r<json["height"].asInt(); ++r) {
        for (auto c = 0; c<json["width"].asInt(); ++c) {
            auto val = json["rows"][r][c].asInt();
            if (val > 0) {
                sprite.setPosition(c * sprW, r * sprH);
                auto quot = val / 16;
                auto rem = val % 16;
                sprite.setTextureRect(sf::IntRect(rem * sprW, quot * sprH, sprW, sprH));
                window.draw(sprite);
            }
        }
    }
    sf::Text text (std::to_string(active_tile), font);
    sf::RectangleShape rect (sf::Vector2f(text.getGlobalBounds().width,text.getGlobalBounds().height+20));
    text.setPosition(0,0);
    text.setColor(sf::Color::White);
    rect.setPosition(text.getPosition());
    rect.setFillColor(sf::Color(0,0,0,128));
    window.draw(rect);
    window.draw(text);
}
