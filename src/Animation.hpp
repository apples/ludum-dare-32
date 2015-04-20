//
// Created by xxAtrain223 on 4/19/2015.
//

#ifndef LUDUMDARE32_ANIMATION_HPP
#define LUDUMDARE32_ANIMATION_HPP

#include "TextureCache.hpp"
#include "echo.hpp"

#include <sfml/Graphics.hpp>
#include <json/json.h>

#include <map>
#include <vector>
#include <fstream>

enum mode {
    loop,
    bounce
};

class Animation : public sf::Sprite {
    struct data {
        std::vector<std::pair<sf::Vector2i, float>> frames;
        mode m;
        int currentFrame = 0;
        int incr = 1;

        data() = default;
        data(std::vector<std::pair<sf::Vector2i, float>> f, mode m) { frames = f; this->m = m; }
    };

    std::map<std::string, data> states;
    data currentState;
    sf::Vector2i frameSize;
    float timer;

public:
    Animation() = default;
    Animation(const sf::Texture &tex) {
        setTexture(tex);
        std::vector<std::pair<sf::Vector2i, float>> frames;
        frames.emplace_back(std::make_pair(sf::Vector2i(0, 0), -1.f));
        states.insert(std::make_pair("default", data(frames, mode::bounce)));
        currentState = states["default"];
        frameSize = sf::Vector2i(tex.getSize().x, tex.getSize().y);
        timer = 0;
    }
    Animation(sf::Sprite spr) : Animation(*spr.getTexture()) { }
    Animation(std::string jsonFile, const sf::Texture &tex) {
        setTexture(tex);
        timer = 0;
        //std::vector<std::pair<sf::Vector2i, float>> frames;

        Json::Value json;
        std::ifstream file(jsonFile);
        file >> json;

        int width = json["width"].asInt();
        int height = json["height"].asInt();
        frameSize = sf::Vector2i(width, height);
        for (auto& name : json.getMemberNames()) {
            if (name == "spritesheet" || name == "width" || name == "height") {
                continue;
            }
            Json::Value anim = json[name];
            mode m;
            if (anim["mode"].asString() == "bounce"){
                m = mode::bounce;
            }
            else if (anim["mode"].asString() == "bounce"){
                m = mode::bounce;
            }
            Json::Value fram = anim["frames"];
            std::vector<std::pair<sf::Vector2i, float>> frames;
            for (Json::Value f : fram) {
                int r = f["row"].asInt();
                int c = f["col"].asInt();
                float dur = f["dur"].asFloat();
                frames.emplace_back(std::make_pair(sf::Vector2i(r, c), dur));
            }
            data d = data(frames, m);
            states.insert(make_pair(name, d));
        }
    }
    Animation(std::string jsonFile, sf::Sprite spr) : Animation(jsonFile, *spr.getTexture()) {}

    sf::Vector2i getFrameSize() const { return frameSize; }
    void setFrameSize(sf::Vector2i fsize) { frameSize = fsize; }
    void setFrameSize(unsigned int x, unsigned int y) { setFrameSize(sf::Vector2i(x, y)); }

    void addState(std::string stateName, mode m, std::vector<std::pair<sf::Vector2i, float>> frames){data d{frames, m};}
    void setState(std::string state) {
        auto found = states.find(state);
        if (found == states.end()) {
            throw std::runtime_error("State '" + state + "' was not found in the animation");
        }
        currentState = found->second;
        currentState.currentFrame = 0;
        ECHO(getTextureRect().left, getTextureRect().top, getTextureRect().width, getTextureRect().height);
        setTextureRect(sf::IntRect(currentState.frames[currentState.currentFrame].first.x * frameSize.x,
                                   currentState.frames[currentState.currentFrame].first.y * frameSize.y,
                                   frameSize.x, frameSize.y));
        ECHO(getTextureRect().left, getTextureRect().top, getTextureRect().width, getTextureRect().height);
    }

    void update(Engine &engine, double time_step) {
        timer += time_step;
        //ECHO(currentState.frames[currentState.currentFrame].second, timer);
        if (currentState.frames[currentState.currentFrame].second != -1 &&
            currentState.frames[currentState.currentFrame].second < timer) {
            timer = 0;
            currentState.currentFrame += currentState.incr;
            if (currentState.m == mode::bounce) {
                if (currentState.currentFrame < 0 ||
                    currentState.currentFrame >= currentState.frames.size()) {
                    currentState.incr *= -1;
                    currentState.currentFrame += currentState.incr;
                }
            }
            else if (currentState.m == mode::loop) {
                if (currentState.currentFrame >= currentState.frames.size()) {
                    currentState.currentFrame = 0;
                }
            }

            setTextureRect(sf::IntRect(currentState.frames[currentState.currentFrame].first.x * frameSize.x,
                                       currentState.frames[currentState.currentFrame].first.y * frameSize.y,
                                       frameSize.x, frameSize.y));
            //ECHO(getTextureRect().left, getTextureRect().top, getTextureRect().width, getTextureRect().height);
        }
    }
};

#endif //LUDUMDARE32_ANIMATION_HPP
