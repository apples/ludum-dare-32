//
// Created by Jeramy on 4/18/2015.
//

#include "Engine.hpp"

#include "TitleScreen.hpp"
#include "MainGame.hpp"

Engine::Engine() {
    window.create(sf::VideoMode(800,600),"Toy Box Escape");
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
}

void Engine::go() {
    states.push(std::make_shared<TitleScreen>());
    clock.restart();
    while (window.isOpen() && !states.empty()) {
        ++current_tick;
        poll_events();
        update();
        draw();
    }
}

void Engine::poll_events() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                keyboard[event.key.code].last_pressed = current_tick;
                ECHO("Key pressed:", event.key.code);
                break;
            case sf::Event::KeyReleased:
                keyboard[event.key.code].last_released = current_tick;
                ECHO("Key released:", event.key.code);
                break;
			case sf::Event::MouseButtonPressed:
				mouseButtons[event.mouseButton.button].last_pressed = current_tick;
				ECHO("Mouse Button pressed:", event.mouseButton.button);
				break;
			case sf::Event::MouseButtonReleased:
			    mouseButtons[event.mouseButton.button].last_released = current_tick;
                ECHO("Mouse Button released:", event.mouseButton.button);
                break;
            case sf::Event::MouseWheelMoved:
                mousePosition.wheel += event.mouseWheel.delta;
                ECHO("Mouse Wheel moved:", event.mouseWheel.delta);
                break;
            case sf::Event::MouseMoved:
                mousePosition.x = event.mouseMove.x;
                mousePosition.y = event.mouseMove.y;
                ECHO("Mouse moved - x:", event.mouseMove.x, "y:", event.mouseMove.y);
                break;

        }
    }
}

void Engine::update() {
    double time_step = clock.getElapsedTime().asSeconds();
    clock.restart();
    states.update(*this, time_step);
}

void Engine::draw() {
    window.clear();
    states.draw(window);
    window.display();
}

bool Engine::isKeyDown(sf::Keyboard::Key key) const {
    return keyboard[key].last_pressed > keyboard[key].last_released;
}

bool Engine::isKeyUp(sf::Keyboard::Key key) const {
    return !isKeyDown(key);
}

bool Engine::wasKeyPressed(sf::Keyboard::Key key) const {
    return keyboard[key].last_pressed == current_tick;
}

bool Engine::wasKeyReleased(sf::Keyboard::Key key) const {
    return keyboard[key].last_released == current_tick;
}

bool Engine::isMouseButtonDown(sf::Mouse::Button button) const {
    return mouseButtons[button].last_pressed > mouseButtons[button].last_released;
}

bool Engine::isMouseButtonUp(sf::Mouse::Button button) const {
    return !isMouseButtonDown(button);
}

bool Engine::wasMouseButtonPressed(sf::Mouse::Button button) const {
    return mouseButtons[button].last_pressed == current_tick;
}

bool Engine::wasMouseButtonReleased(sf::Mouse::Button button) const {
    return mouseButtons[button].last_released == current_tick;
}

