#include <SFML/Graphics.hpp>

#include <Game/TronGame.h>

#include "Client.h"
#include "InputHandler.h"
#include "GameAction.h"

Client::Client()
    : tron_game(std::make_unique<TronGame>())
    , input_handler(nullptr)
{
}

void Client::run()
{
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1280, 720), "Tron Game");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    input_handler = std::make_unique<InputHandler>(*window, *tron_game);
    input_handler->register_key(sf::Keyboard::Key::Escape, GameAction::QUIT);

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            handle_event(event);
        }

        window->clear();
        window->draw(shape);
        window->display();
    }
}

void Client::handle_event(sf::Event& _event) const
{
    // Input Handler gets first dibs at handling events.
    if (input_handler->handle_input(_event))
        return;

    if (_event.type == sf::Event::Closed)
        window->close();
}
