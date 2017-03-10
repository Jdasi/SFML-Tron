#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>

#include <Game/TronGame.h>
#include <Game/PacketID.h>
#include "TronClient.h"
#include "InputHandler.h"
#include "GameAction.h"
#include "ClientStates.h"

TronClient::TronClient()
{
}

void TronClient::run()
{
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Tron Game");

    // Declare and load a font.
    sf::Font font;
    if (!font.loadFromFile("../../Resources/arial.ttf"))
    {
        return;
    }

    object_renderer = std::make_unique<ObjectRenderer>(*window);

    // Set up some key bindings.
    input_handler = std::make_unique<InputHandler>(*this);
    input_handler->registerKey(sf::Keyboard::Key::Escape, GameAction::QUIT);
    input_handler->registerKey(sf::Keyboard::Key::Return, GameAction::ACCEPT);

    // States require Client Data to be set up.
    client_data = std::make_unique<ClientData>(&font, input_handler.get(), object_renderer.get());

    state_handler = std::make_unique<ClientStateHandler>();
    state_handler->registerState("GameStart", std::make_unique<ClientStateStart>(client_data.get()));
    state_handler->registerState("GamePlay", std::make_unique<ClientStateGame>(client_data.get()));

    state_handler->queueState("GameStart");

    while (window->isOpen() && !client_data->exit)
    {
        // Crude delta-time system.
        client_data->delta_time = timer.getTimeDifference();
        timer.reset();
        client_data->play_time += client_data->delta_time;

        sf::Event event;
        while (window->pollEvent(event))
        {
            handleEvent(event);
        }

        object_renderer->draw();
        state_handler->tick();
    }

    window->close();
}

void TronClient::onCommand(GameAction _action, ActionState _action_state) const
{
    state_handler->onCommand(_action, _action_state);
}

void TronClient::handleEvent(const sf::Event& _event) const
{
    // Input Handler gets first dibs at handling events.
    if (input_handler->handleInput(_event))
    {
        return;
    }

    if (_event.type == sf::Event::Closed)
    {
        client_data->exit = true;
        window->close();
    }
}
