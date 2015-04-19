//
// Created by Jeramy on 4/19/2015.
//

#include "LevelEditor.hpp"
#include "echo.hpp"

#include "Engine.hpp"

LevelEditor::LevelEditor() {
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
    if (engine.wasMouseButtonPressed(sf::Mouse::Left)) {

    }
}

void LevelEditor::draw(sf::RenderWindow &window) const {
    sf::Sprite sprite;
    auto& tex = textures.get("terrain");
    sprite.setTexture(tex);
    for (auto r = 0; r<json["height"].asInt(); ++r) {
        for (auto c = 0; c<json["width"].asInt(); ++c) {
            sprite.setPosition(c*16,r*16);
            auto tsz = tex.getSize();
            auto sprW = tsz.x/16;
            auto sprH = tsz.y/16;
            auto val = json["rows"][r][c].asInt();
            auto quot = val/16;
            auto rem = val%16;
            sprite.setTextureRect(sf::IntRect(rem*sprW,quot*sprH,sprW,sprH));
            window.draw(sprite);
        }
    }
}
