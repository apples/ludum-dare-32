//
// Created by Jeramy on 4/19/2015.
//

#include "LevelEditor.hpp"
#include "echo.hpp"

LevelEditor::LevelEditor() {
    json["width"] = 10;
    json["height"] = 10;
    json["rows"].resize(json["height"].asInt());
    for (auto i = 0; i<json["height"].asInt(); ++i) {
        json["rows"][i].resize(json["width"].asInt());
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

}

void LevelEditor::draw(sf::RenderWindow &window) const {
    sf::Sprite sprite;
    for (auto r = 0; r<json["height"].asInt(); ++r) {
        for (auto c = 0; c<json["width"].asInt(); ++c) {
            sprite.setPosition(c*16,r*16);
            sprite.setTexture(textures.get("terrain"));
            sprite.setTextureRect(sf::IntRect(0,0,16,16));
            window.draw(sprite);
        }
    }
}
