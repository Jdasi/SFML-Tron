#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>

#include <Game/TronGame.h>
#include <Game/PacketID.h>
#include "TronClient.h"
#include "InputHandler.h"
#include "GameAction.h"

class Game : public ClientState
{
public:
    Game() = default;
    virtual ~Game() = default;

    void onCommand(GameAction _action, ActionState _action_state) override {}
    void onStateEnter() override {}
    void onStateLeave() override {}
    void tick() override {}
};

TronClient::TronClient(const std::string& _ip_address, unsigned int _port)
    : delta_time(0)
    , ip_address(sf::IpAddress(_ip_address))
    , tcp_port(_port)
    , exit(false)
    , latency(0)
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

    tron_game = std::make_unique<TronGame>();
    object_renderer = std::make_unique<ObjectRenderer>(*window);

    input_handler = std::make_unique<InputHandler>(*this);
    input_handler->registerKey(sf::Keyboard::Key::Escape, GameAction::QUIT);

    state_handler = std::make_unique<ClientStateHandler>();
    state_handler->registerState("GamePlay", std::move(std::make_unique<Game>()));
    state_handler->queueState("GamePlay");

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    object_renderer->link(shape);

    // Create text.
    sf::Text text("hello", font);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);
    object_renderer->link(text);

    if (!connect())
    {
        return;
    }

    std::thread listen_thread(&TronClient::listen, this);
    std::thread ping_thread(&TronClient::ping, this);

    while (window->isOpen() && !exit)
    {
        // Crude delta-time system.
        delta_time = timer.getTimeDifference();
        timer.reset();

        sf::Event event;
        while (window->pollEvent(event))
        {
            handleEvent(event);
        }

        text.setString(std::to_string(latency) + "us");

        object_renderer->draw();
        state_handler->tick();
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
            shutdown();
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
        shutdown();
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

void TronClient::listen()
{
    sf::Socket::Status status = sf::Socket::Done;
    while (status != sf::Socket::Disconnected && !exit)
    {
        sf::Packet packet;
        status = socket.receive(packet);

        if (status == sf::Socket::Done)
        {
            handlePacket(packet);
        }
        else
        {
            exit = true;
        }
    }
}

void TronClient::ping()
{
    sf::Socket::Status status = sf::Socket::Done;
    while (status != sf::Socket::Disconnected && !exit)
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
                exit = true;
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

void TronClient::shutdown()
{
    sf::Packet packet;
    setPacketID(packet, PacketID::DISCONNECT);

    socket.send(packet);
    exit = true;
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
