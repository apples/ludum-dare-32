#ifndef BALL_HPP
#define BALL_HPP

#include "thing.hpp"

struct Ball : Thing {

	sf::CircleShape circ;
	sf::Vector2f pos;
	sf::Vector2f vel;

	Ball() : circ(10.f), pos(400-10.f, 300-10.f)//, vel(7.f, 7.f)
	{
		circ.setFillColor(sf::Color::White);
		circ.setPosition(pos);
	}

    void draw(sf::RenderWindow& window) override {
		window.draw(circ);
	}

    void tick() override {
		pos += vel;
		circ.setPosition(pos);
	}
};

#endif // BALL_HPP
