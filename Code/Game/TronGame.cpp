#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "TronGame.h"

TronGame::TronGame()
    : input_handler_(std::make_unique<InputHandler>())
{
}

void TronGame::run()
{
    sf::RenderWindow window({ 1280, 720 }, "Tron Game");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}

sf::Packet& operator<<(sf::Packet& packet, const TronGame& game)
{
	return packet;
}

sf::Packet& operator >> (sf::Packet& packet, const TronGame& game)
{
	return packet;
}
