#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>

#include <Game/TronGame.h>
#include <Game/PacketID.h>
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
    , waiting_for_pong(false)
    , latency(0)
{
}

void TronClient::run()
{
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1280, 720), "Tron Game");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    // Declare and load a font.
    sf::Font font;
    if (!font.loadFromFile("../../Resources/arial.ttf"))
    {
        return;
    }

    // Create text.
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

    std::thread listen_thread(&TronClient::listen, this);
    std::thread ping_thread(&TronClient::ping, this);

    while (window->isOpen() && !exit)
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            handleEvent(event);
        }

        text.setString(std::to_string(latency) + "us");

        window->clear();
        window->draw(shape);
        window->draw(text);
        window->display();
    }

    listen_thread.join();
    ping_thread.join();

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

    sf::Packet packet;
    packet >> "Test";

    return true;
}

void TronClient::listen()
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
}

void TronClient::ping()
{
    while (socket_status != sf::Socket::Disconnected && !exit)
    {
        if (waiting_for_pong)
        {
            continue;
        }

        // Ping thread should only occur once a second to minimise network traffic.
        std::this_thread::sleep_for(std::chrono::seconds(1));

        sf::Packet packet;
        setPacketID(packet, PacketID::PING);

        ping_sent_point = std::chrono::steady_clock::now();
        socket_status = socket.send(packet);

        waiting_for_pong = true;
    }
}

void TronClient::disconnect()
{
    sf::Packet packet;
    setPacketID(packet, PacketID::DISCONNECT);

    socket_status = socket.send(packet);
    if (socket_status == sf::Socket::Done || 
        socket_status == sf::Socket::Disconnected)
    {
        exit = true;
    }
}

void TronClient::handlePacket(sf::Packet& _packet)
{
    PacketID pid = getPacketID(_packet);

    switch (pid)
    {
        case DISCONNECT:
        {
            disconnect();
        } break;

        // Determine latency to server.
        case PONG:
        {
            latency = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - ping_sent_point).count();

            sf::Packet packet;
            setPacketID(packet, PacketID::LATENCY);

            packet << static_cast<sf::Uint64>(latency);
            socket_status = socket.send(packet);

            waiting_for_pong = false;
        } break;

        case MESSAGE:
        {
            std::string str;
            _packet >> str;
            std::cout << str << std::endl;
        } break;

        default: {}
    }
}
