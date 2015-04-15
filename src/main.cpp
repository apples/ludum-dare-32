#include <SFML/Graphics.hpp>
#include "thing.hpp"
#include "ball.hpp"
#include "player.hpp"

#include <vector>
#include <memory>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");
	window.setVerticalSyncEnabled(true);

    std::vector<std::shared_ptr<Thing>> things;

    things.push_back(std::make_shared<Player>(1));
    things.push_back(std::make_shared<Player>(2));
    things.push_back(std::make_shared<Ball>());

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                }
				case sf::Event::KeyPressed: {
					if (event.key.code == sf::Keyboard::Escape)
						window.close();
				}
            }
        }

        for (auto& ptr : things) {
            ptr->tick();
        }

        window.clear();
        for (auto& ptr : things) {
            ptr->draw(window);
        }
        window.display();
    }

    return 0;
}
