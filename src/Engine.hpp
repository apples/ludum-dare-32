//
// Created by Jeramy on 4/18/2015.
//

#ifndef LUDUMDARE32_ENGINE_HPP
#define LUDUMDARE32_ENGINE_HPP

#include "GameStateStack.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>

#ifndef NDEBUG
#include <iostream>
template <typename Head>
void ECHO(const Head& head) {
    std::clog << head << std::endl;
}
template <typename Head, typename... Tail>
void ECHO(const Head& head, const Tail&... tail) {
    std::clog << head << " ";
    ECHO(tail...);
}
#else
template <typename... Ts>
void ECHO(Ts&&...) {}
#endif

class Engine {
    struct KeyState {
        int last_pressed = -1;
        int last_released = -1;
    };
	
	struct MouseButtonState {
		int last_pressed = -1;
		int last_released = -1;
	};

    struct MousePosition {
        int x;
        int y;
        int wheel = 0;
    };


    sf::RenderWindow window;

    sf::Clock clock;

    int current_tick = 0;
    std::vector<KeyState> keyboard = std::vector<KeyState>(sf::Keyboard::KeyCount);
	std::vector<MouseButtonState> mouseButtons = std::vector<MouseButtonState>(sf::Mouse::ButtonCount);
    MousePosition mousePosition;

    void poll_events();
    void update();
    void draw();

public:
    Engine();
    void go();

    bool isKeyDown(sf::Keyboard::Key key) const;
    bool isKeyUp(sf::Keyboard::Key key) const;
    bool wasKeyPressed(sf::Keyboard::Key key) const;
    bool wasKeyReleased(sf::Keyboard::Key key) const;
	
	bool isMouseButtonDown(sf::Mouse::Button button) const;
    bool isMouseButtonUp(sf::Mouse::Button button) const;
    bool wasMouseButtonPressed(sf::Mouse::Button button) const;
    bool wasMouseButtonReleased(sf::Mouse::Button button) const;

    const MousePosition& getMousePosition() const {
        return mousePosition;
    }

    GameStateStack states;
};


#endif //LUDUMDARE32_ENGINE_HPP
