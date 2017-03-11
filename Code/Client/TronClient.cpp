#include <iostream>
#include <thread>
#include <functional>

#include <SFML/Graphics.hpp>

#include "TronClient.h"

TronClient::TronClient(sf::IpAddress _ip_address, unsigned int _tcp_port)
    : network_manager(*this, _ip_address, _tcp_port)
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

    network_manager.connect();

    object_renderer = std::make_unique<ObjectRenderer>(*window);
    input_handler = std::make_unique<InputHandler>(*this);

    // Set up some key bindings.
    input_handler->registerKey(sf::Keyboard::Key::Escape, GameAction::QUIT);
    input_handler->registerKey(sf::Keyboard::Key::Return, GameAction::ACCEPT);

    // States require Client Data to be set up.
    client_data = std::make_unique<ClientData>(&font, input_handler.get(), object_renderer.get());

    state_handler = std::make_unique<ClientStateHandler>();
    state_handler->registerState("GameStart", std::make_unique<ClientStateStart>(client_data.get()));
    state_handler->registerState("GamePlay", std::make_unique<ClientStateGame>(client_data.get()));

    state_handler->queueState("GameStart");

    //std::thread network_thread(&TronClient::network, this);

    while (window->isOpen() && !client_data->exit)
    {
        // Crude delta-time system.
        client_data->delta_time = timer.getTimeDifference();
        timer.reset();
        client_data->play_time += client_data->delta_time;

        // Execute everything in the client queue
        executeDispatchedMethods();

        // Update and draw.
        state_handler->tick();
        object_renderer->draw();

        // Handle events.
        sf::Event event;
        while (window->pollEvent(event))
        {
            handleEvent(event);
        }
    }

    //network_thread.join();

    window->close();
}

void TronClient::onCommand(GameAction _action, ActionState _action_state) const
{
    state_handler->onCommand(_action, _action_state);
}

void TronClient::updatePingTime(const double ping)
{
    postEvent([this, ping]()
    {
        // do something with ping? put it on the screen?
        std::cout << "Ping: " << ping << std::endl;
    });
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
