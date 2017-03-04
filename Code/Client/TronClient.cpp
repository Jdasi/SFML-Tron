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

    // Declare and load a font
    sf::Font font;
    if (!font.loadFromFile("../../Resources/arial.ttf"))
    {
        return;
    }

    // Create a text
    sf::Text text("hello", font);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);

    input_handler = std::make_unique<InputHandler>(*this);
    input_handler->registerKey(sf::Keyboard::Key::Escape, GameAction::QUIT);

    if (!connect())
    {
        return;
    }

    auto handle = std::async(std::launch::async, [&]
    {
        while (socket_status != sf::Socket::Disconnected && !exit)
        {
            sf::Packet packet;
            socket_status = socket.receive(packet);

            if (socket_status == sf::Socket::Done)
            {
                handlePacket(packet);
            }
        }
    });

    while (window->isOpen() && !exit)
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            handleEvent(event);
        }

        window->clear();
        window->draw(shape);
        window->draw(text);
        window->display();
    }

    window->close();
}

void TronClient::onCommand(GameAction _action, ActionState _action_state)
{
    if (_action == GameAction::QUIT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            disconnect();
        }
    }
}

void TronClient::handleEvent(sf::Event& _event)
{
    // Input Handler gets first dibs at handling events.
    if (input_handler->handleInput(_event))
    {
        return;
    }

    if (_event.type == sf::Event::Closed)
    {
        disconnect();
    }
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

void TronClient::disconnect()
{
    std::string msg("disconnectme");
    sf::Packet packet;
    packet << msg;

    if (socket.send(packet) == sf::Socket::Done)
    {
        exit = true;
        window->close();
    }
}

void TronClient::handlePacket(sf::Packet& _packet) const
{
    std::string message;
    _packet >> message;
    std::cout << message << std::endl;
}
