//
// Created by xxAtrain223 on 4/18/2015.
//

#include "GraphicsSandbox.hpp"

GraphicsSandbox::GraphicsSandbox() {
}

bool GraphicsSandbox::halts_update() const {
    return true;
}

bool GraphicsSandbox::halts_draw() const {
    return true;
}

void GraphicsSandbox::update(Engine &engine, double time_step) {

}

void GraphicsSandbox::draw(sf::RenderWindow &window) const {
    sf::RectangleShape rect(sf::Vector2f(200, 400));
    rect.setFillColor(sf::Color::Blue);
    window.draw(rect);
}