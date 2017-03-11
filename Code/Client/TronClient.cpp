#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>

#include "TronClient.h"

TronClient::TronClient(sf::IpAddress _ip_address, unsigned int _tcp_port)
    : ip_address(_ip_address)
    , tcp_port(_tcp_port)
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

    if (!connect())
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

    std::thread receive_thread(&TronClient::receive, this);
    std::thread ping_thread(&TronClient::ping, this);

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

    receive_thread.join();
    ping_thread.join();

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

void TronClient::receive()
{
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

void TronClient::ping()
{
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

            packet << client_data->latency;
            if (socket.send(packet) == sf::Socket::Disconnected)
            {
                client_data->exit = true;
            }
        }

        // Minimise network traffic.
        std::this_thread::sleep_for(std::chrono::seconds(1));

        sf::Packet packet;
        setPacketID(packet, PacketID::PING);

        packet << client_data->play_time;
        status = socket.send(packet);
    }
}

void TronClient::shutdown()
{
    sf::Packet packet;
    setPacketID(packet, PacketID::DISCONNECT);

    socket.send(packet);
}

void TronClient::handlePacket(sf::Packet& _packet)
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
            double prev_play_time = 0;
            _packet >> prev_play_time;

            client_data->latency = static_cast<sf::Uint64>((client_data->play_time - prev_play_time) * 1000);

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
