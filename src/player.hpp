#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "thing.hpp"

struct Player : Thing {
	int pid;
	sf::RectangleShape rect;

	int y = 300 - 50 / 2;

	sf::Keyboard::Key upKey;
	sf::Keyboard::Key downKey;

	Player(int id) : pid(id), rect(sf::Vector2f(5, 50)) {
		rect.setFillColor(sf::Color(255, 255, 255));
		upKey = pid == 1 ? sf::Keyboard::Key::W : sf::Keyboard::Key::Up;
		downKey = pid == 1 ? sf::Keyboard::Key::S : sf::Keyboard::Key::Down;
	}

	void draw(sf::RenderWindow& window) override {
		rect.setPosition(sf::Vector2f((pid == 1 ? 20 : 800 - 20), y));
		window.draw(rect);
	}

	void tick() override {
		if (sf::Keyboard::isKeyPressed(upKey)) {
			y -= 5;
		}
		if (sf::Keyboard::isKeyPressed(downKey)) {
			y += 5;
		}
		if (y < 0) y = 0;
		if (y > 600 - rect.getLocalBounds().height) y = 600 - rect.getLocalBounds().height;
	}
};

#endif // PLAYER_HPP
