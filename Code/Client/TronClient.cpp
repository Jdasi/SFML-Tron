#include <iostream>
#include <future>

#include <SFML/Graphics.hpp>

#include <Game/TronGame.h>
#include "TronClient.h"
#include "InputHandler.h"
#include "GameAction.h"

TronClient::TronClient(const std::string& _ip_address, unsigned int _port)
    : tron_game(std::make_unique<TronGame>())
    , input_handler(nullptr)
    , ip_address(sf::IpAddress(_ip_address))
    , tcp_port(_port)
    , socket_status(sf::Socket::Done)
    , exit(false)
{
}

void TronClient::run()
{
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1280, 720), "Tron Game");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    input_handler = std::make_unique<InputHandler>(*window, *tron_game);
    input_handler->register_key(sf::Keyboard::Key::Escape, GameAction::QUIT);

    if (!connect())
    {
        return;
    }

    auto handle = std::async(std::launch::async, [&]
    {
        while (socket_status != sf::Socket::Disconnected)
        {
            sf::Packet packet;
            socket_status = socket.receive(packet);

            if (socket_status == sf::Socket::Done)
            {
                handlePacket(packet);
            }
        }
    });

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

void TronClient::handle_event(sf::Event& _event) const
{
    // Input Handler gets first dibs at handling events.
    if (input_handler->handle_input(_event))
        return;

    if (_event.type == sf::Event::Closed)
        window->close();
}

bool TronClient::connect()
{
    auto status = socket.connect(ip_address, tcp_port);
    if (status != sf::Socket::Done)
    {
        return false;
    }

    std::cout << "Connected to server: " << ip_address << std::endl;

    return true;
}

void TronClient::handlePacket(sf::Packet& packet) const
{
    std::string message;
    packet >> message;
    std::cout << message << std::endl;
}