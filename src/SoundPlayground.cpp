//
// Created by xxAtrain223 on 4/20/2015.
//

#include "SoundPlayground.hpp"

SoundPlayground::SoundPlayground() {

}

bool SoundPlayground::halts_update() const {
    return true;
}

bool SoundPlayground::halts_draw() const {
    return true;
}

void SoundPlayground::update(Engine &engine, double time_step) {
    if (engine.wasKeyPressed(sf::Keyboard::Escape)) {
        engine.states.pop();
        return;
    }

    if (engine.wasKeyPressed(sf::Keyboard::Num1)) {
        engine.music.setNextSong("intro");
    }
    if (engine.wasKeyPressed(sf::Keyboard::Num2)) {
        engine.music.setNextSong("partA");
    }
    if (engine.wasKeyPressed(sf::Keyboard::Num3)) {
        engine.music.setNextSong("bossFight");
    }


}

void SoundPlayground::draw(sf::RenderWindow &window) const {
    window.clear(sf::Color(64, 64, 255));
}
