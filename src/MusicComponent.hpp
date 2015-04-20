//
// Created by xxAtrain223 on 4/20/2015.
//

#ifndef LUDUMDARE32_MUSICCOMPONENT_HPP
#define LUDUMDARE32_MUSICCOMPONENT_HPP

#include "echo.hpp"

#include <sfml/Audio.hpp>
#include <json/json.h>

#include <map>
#include <vector>
#include <fstream>

class MusicComponent {
    struct songData{
        std::string fileName;
        int bpm;
        std::vector<int> bars;
        bool loop;
    };

    sf::Music music;
    std::map<std::string, songData> library;
    std::string currentSong;
    std::string nextSong;
    int switchBar;

public:
    void loadJson(std::string jsonFileName) {
        Json::Value json;
        std::ifstream file("data/"+jsonFileName);
        file >> json;

        for (auto& name : json.getMemberNames()) {
            Json::Value jsonSong = json[name];
            songData data;
            data.fileName = jsonSong["filename"].asString();
            data.bpm = jsonSong["bpm"].asInt();
            Json::Value bars = jsonSong["bars"];
            for (auto& bar : bars) {
                data.bars.emplace_back(bar.asInt());
            }
            data.loop = jsonSong["loop"].asBool();

            library.insert(std::make_pair(name, data));
        }

        switchBar = 0;
    }

    void play(std::string songName) {
        currentSong = songName;

        music.openFromFile("data/" + library[currentSong].fileName);
        music.setLoop(library[currentSong].loop);
        music.play();
    }

    void setNextSong(std::string songName) {
        if (currentSong != "") {
            nextSong = songName;
            if (music.getStatus() == sf::Music::Playing) {
                for (int b : library[currentSong].bars) {
                    if (library[currentSong].bpm != -1) {
                        sf::Time t = music.getPlayingOffset();
                        if (sf::seconds(b * 4.f / library[currentSong].bpm * 60.f) > t) {
                            switchBar = b;
                            break;
                        }
                    }
                }
            }
            else {
                currentSong = songName;
                play(currentSong);
            }
        }
        else {
            currentSong = songName;
            play(currentSong);
        }
    }

    void update(){
        if (nextSong != "") {
            sf::Time t = music.getPlayingOffset();
            if (library[currentSong].bpm == -1) {
                for (int b : library[currentSong].bars) {
                    if (t.asSeconds() > b)/* || )*/ {
                        play(nextSong);
                        nextSong = "";
                    }
                }
            }
            else {
                if (t > sf::seconds(switchBar * 4.f / library[currentSong].bpm * 60.f)) {
                    play(nextSong);
                    nextSong = "";
                }
            }
        }
    }

    sf::Music::Status getStatus() {
        return music.getStatus();
    }

    std::string getCurrentSong() {
        return currentSong;
    }

    std::string getNextSong() {
        return nextSong;
    }
};

#endif //LUDUMDARE32_MUSICCOMPONENT_HPP
