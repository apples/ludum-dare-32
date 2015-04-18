//
// Created by Jeramy on 4/18/2015.
//

#include "Engine.hpp"

#include "TitleScreen.hpp"

Engine::Engine() {
    window.create(sf::VideoMode(800,600),"Toy Box Escape");
    window.setVerticalSyncEnabled(true);
}

void Engine::go() {
    states.push(TitleScreen{});
    while (window.isOpen()) {
        poll_events();
        update();
        draw();
    }
}

void Engine::poll_events() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                window.close();
            }
                break;
            case sf::Event::KeyPressed: {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
                break;
        }
    }
}

void Engine::update() {
    states.update();
}

void Engine::draw() {
    window.clear();
    states.draw(window);
    window.display();
}
