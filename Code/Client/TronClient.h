#pragma once
#include <memory>
#include <atomic>
#include <chrono>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <Game/TronGame.h>
#include "InputHandler.h"
#include "SimpleTimer.h"

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
    std::unique_ptr<InputHandler> input_handler;

    SimpleTimer timer;
    float delta_time;
    float reattempt_timer;
    float reattempt_threshold;

    sf::IpAddress ip_address;
    unsigned int tcp_port;
    sf::TcpSocket socket;
    
    std::atomic<bool> exit;
    std::string user_name;

    std::atomic<bool> waiting_for_pong;
    std::chrono::steady_clock::time_point ping_sent_point;
    __int64 latency;

};
