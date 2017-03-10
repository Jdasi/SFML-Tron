#include <iostream>
#include <thread>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <Game/Constants.h>
#include <Game/PacketID.h>
#include "ClientStateGame.h"
#include "ClientData.h"
#include "ObjectRenderer.h"
#include "ClientStateHandler.h"

ClientStateGame::ClientStateGame(ClientData* _client_data)
    : ClientState(_client_data)
    , networking(false)
    , latency_text(nullptr)
    , receive_thread(&ClientStateGame::receive, this)
    , ping_thread(&ClientStateGame::ping, this)
    , ip_address(SERVER_IP)
    , tcp_port(SERVER_TCP_PORT)
    , latency(0)
{
}

ClientStateGame::~ClientStateGame()
{
    receive_thread.join();
    ping_thread.join();
}

void ClientStateGame::onStateEnter()
{
    networking = true;

    if (!connect())
    {
        client_data->exit = true;
        // should probably return here, but then you need to do nullptr checks in tick
    }

    // Create text.
    auto text = std::make_unique<sf::Text>("", *client_data->font);
    text->setCharacterSize(30);
    text->setStyle(sf::Text::Bold);
    text->setFillColor(sf::Color::Red);

    latency_text = text.get();
    objects.push_back(std::move(text));

    for (auto& obj : objects)
    {
        client_data->object_renderer->link(*obj);
    }
}

void ClientStateGame::onStateLeave()
{
    for (auto& obj : objects)
    {
        client_data->object_renderer->unlink(*obj);
    }

    shutdown();
}

void ClientStateGame::tick()
{
    latency_text->setString(std::to_string(latency) + "us");
}

void ClientStateGame::onCommand(const GameAction _action, const ActionState _action_state)
{
    if (_action == GameAction::QUIT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            getHandler()->queueState("GameStart");
        }
    }
}

bool ClientStateGame::connect()
{
    auto status = socket.connect(ip_address, tcp_port);
    if (status != sf::Socket::Done)
    {
        return false;
    }

    std::cout << "Connected to server: " << ip_address << std::endl;

    return true;
}

void ClientStateGame::receive()
{
    while (!networking && !client_data->exit)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    sf::Socket::Status status = sf::Socket::Done;
    while (status != sf::Socket::Disconnected && !client_data->exit)
    {
        sf::Packet packet;
        status = socket.receive(packet);

        if (status == sf::Socket::Done)
        {
            handlePacket(packet);
        }
    }
}

void ClientStateGame::ping()
{
    while (!networking && !client_data->exit)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    sf::Socket::Status status = sf::Socket::Done;
    while (status != sf::Socket::Disconnected && !client_data->exit)
    {
        {
            std::lock_guard<std::mutex> guard(pong_queue_mutex);
            if (pong_queue.empty())
            {
                continue;
            }

            pong_queue.pop();

            sf::Packet packet;
            setPacketID(packet, PacketID::LATENCY);

            packet << static_cast<sf::Uint64>(latency);
            if (socket.send(packet) == sf::Socket::Disconnected)
            {
                client_data->exit = true;
            }
        }

        // Minimise network traffic.
        std::this_thread::sleep_for(std::chrono::seconds(1));

        sf::Packet packet;
        setPacketID(packet, PacketID::PING);

        ping_sent_point = std::chrono::steady_clock::now();
        status = socket.send(packet);
    }
}

void ClientStateGame::shutdown()
{
    sf::Packet packet;
    setPacketID(packet, PacketID::DISCONNECT);

    socket.send(packet);
}

void ClientStateGame::handlePacket(sf::Packet& _packet)
{
    PacketID pid = getPacketID(_packet);

    switch (pid)
    {
    case DISCONNECT:
    {
        shutdown();
    } break;

    // Determine latency to server.
    case PONG:
    {
        latency = std::chrono::duration_cast<std::chrono::microseconds>
            (std::chrono::steady_clock::now() - ping_sent_point).count();

        {
            std::lock_guard<std::mutex> guard(pong_queue_mutex);
            pong_queue.push(_packet);
        }
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