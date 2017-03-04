#pragma once
#include <memory>
#include <atomic>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <Game/TronGame.h>
#include "InputHandler.h"

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
    void disconnect();
    void handlePacket(sf::Packet& _packet) const;

    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<TronGame> tron_game;
    std::unique_ptr<InputHandler> input_handler;

    sf::IpAddress ip_address;
    unsigned int tcp_port;
    sf::TcpSocket socket;
    std::atomic<sf::Socket::Status> socket_status;
    std::atomic<bool> exit;
    std::string user_name;

};
