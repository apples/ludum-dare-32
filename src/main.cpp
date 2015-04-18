#include <SFML/Graphics.hpp>

#include "entcom.hpp"
#include "components.hpp"

int main()
{
    using namespace components;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");
	window.setVerticalSyncEnabled(true);

    DB db {};

    EntID player1;
    EntID player2;

    player1 = db.makeEntity();
    db.makeComponent(player1, Player{1,20});
    db.makeComponent(player1, Position{{20,300}});
    db.makeComponent(player1, Velocity{});
    db.makeComponent(player1, Image{PlayerPainter{}});
    db.makeComponent(player1, AI{PlayerBrain{
            []{return sf::Keyboard::isKeyPressed(sf::Keyboard::W);},
            []{return sf::Keyboard::isKeyPressed(sf::Keyboard::S);}
    }});

    player2 = db.makeEntity();
    db.makeComponent(player2, Player{2,800-20});
    db.makeComponent(player2, Position{{800-20,300}});
    db.makeComponent(player2, Velocity{});
    db.makeComponent(player2, Image{PlayerPainter{}});
    db.makeComponent(player2, AI{PlayerBrain{
            []{return sf::Keyboard::isKeyPressed(sf::Keyboard::Up);},
            []{return sf::Keyboard::isKeyPressed(sf::Keyboard::Down);}
    }});

    // Add a ball
    {
        EntID ball = db.makeEntity();
        db.makeComponent(ball, Ball{});
        db.makeComponent(ball, Position{{400,300}});
        db.makeComponent(ball, Velocity{});
        db.makeComponent(ball, Image{BallPainter{}});
        db.makeComponent(ball, AI{BallBrain{}});
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                } break;
				case sf::Event::KeyPressed: {
					if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
				} break;
            }
        }

        for (auto& ent : db.query<AI>()) {
            std::get<1>(ent).data().brain(db,std::get<0>(ent));
        }

        for (auto& ent : db.query<Position,Velocity>()) {
            auto& pos = std::get<1>(ent).data().pos;
            auto& vel = std::get<2>(ent).data().vel;
            pos += vel;
            vel = sf::Vector2f(0,0);
        }

        window.clear();
        for (auto& ent : db.query<Image>()) {
            std::get<1>(ent).data().painter(std::get<0>(ent),window);
        }
        window.display();
    }

    return 0;
}
