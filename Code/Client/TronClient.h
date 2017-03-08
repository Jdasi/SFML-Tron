#pragma once
#include <memory>
#include <atomic>
#include <chrono>
#include <queue>
#include <mutex>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <Game/TronGame.h>
#include "InputHandler.h"
#include "SimpleTimer.h"
#include "ObjectRenderer.h"
#include "ClientStateHandler.h"

class TronClient
{
public:
    TronClient(const std::string& _ip_address, unsigned int _port);
    ~TronClient() = default;

    void run();
    void onCommand(GameAction _action, ActionState _action_state);

private:
    void handleEvent(sf::Event& _event);
    bool connect();
    void listen();
    void ping();
    void shutdown();
    void handlePacket(sf::Packet& _packet);

    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<TronGame> tron_game;
    std::unique_ptr<ObjectRenderer> object_renderer;
    std::unique_ptr<InputHandler> input_handler;
    std::unique_ptr<ClientStateHandler> state_handler;

    SimpleTimer timer;
    float delta_time;
    std::queue<sf::Packet> pong_queue;
    std::mutex pong_queue_mutex;

    sf::IpAddress ip_address;
    unsigned int tcp_port;
    sf::TcpSocket socket;
    
    std::atomic<bool> exit;
    std::string user_name;

    std::chrono::steady_clock::time_point ping_sent_point;
    __int64 latency;

};
