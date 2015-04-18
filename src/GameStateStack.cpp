//
// Created by Jeramy on 4/18/2015.
//

#include "GameStateStack.hpp"

void GameStateStack::push(std::shared_ptr<GameState> ptr) {
    states.push_back(std::move(ptr));
}

void GameStateStack::pop() {
    states.pop_back();
}

void GameStateStack::update() {
    for (auto iter = states.rbegin(), eiter = states.rend(); iter != eiter; ++iter) {
        auto& state = *iter;
        bool halts = state->halts_update();
        state->update();
        if (halts) {
            return;
        }
    }
}

void GameStateStack::draw(sf::RenderWindow& window) const {
    auto find_start = [&] {
        for (auto riter = states.rbegin(), eriter = states.rend(); riter != eriter; ++riter) {
            auto iter = next(riter).base();
            auto halts = (*iter)->halts_draw();
            if (halts) {
                return iter;
            }
        }
        return states.begin();
    };

    for (auto iter=find_start(), eiter=states.end(); iter!=eiter; ++iter) {
        (*iter)->draw(window);
    }
}
