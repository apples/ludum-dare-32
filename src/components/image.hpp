#ifndef LUDUMDARE32_IMAGE_HPP
#define LUDUMDARE32_IMAGE_HPP

#include <SFML/Graphics.hpp>

namespace components {

struct Image {
    std::function<void(EntID,sf::RenderWindow&)> painter;
};

struct PlayerPainter {
    sf::RectangleShape rect;
    PlayerPainter() {
        rect.setSize(sf::Vector2f(10,50));
        rect.setFillColor(sf::Color::White);
    }
    void operator()(EntID me, sf::RenderWindow& window) {
        rect.setPosition(me.get<Position>().data().pos);
        window.draw(rect);
    }
};

struct BallPainter {
    sf::CircleShape circ;
    BallPainter() {
        circ.setRadius(10);
        circ.setFillColor(sf::Color::White);
    }
    void operator()(EntID me, sf::RenderWindow& window) {
        circ.setPosition(me.get<Position>().data().pos);
        window.draw(circ);
    }
};

} // namespace components

#endif //LUDUMDARE32_IMAGE_HPP
