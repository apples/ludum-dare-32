//
// Created by Jeramy on 4/18/2015.
//

#ifndef LUDUMDARE32_GAMESTATESTACK_H
#define LUDUMDARE32_GAMESTATESTACK_H

#include "GameState.hpp"

#include <vector>
#include <memory>
#include <utility>

class GameStateStack : public GameState {
    std::vector<std::shared_ptr<GameState>> states;
public:
    void push(std::shared_ptr<GameState> ptr);
    void pop();
    void update() override;
    void draw(sf::RenderWindow &window) const override;

    template<typename T>
    void push(T &&t) { return push(std::make_shared<T>(std::forward<T>(t))); }
    template<typename T>
    void push(std::shared_ptr<T> ptr) { return push(std::static_pointer_cast<GameState>(std::move(ptr))); }
};


#endif //LUDUMDARE32_GAMESTATESTACK_H
