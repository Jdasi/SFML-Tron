#pragma once
#include <memory>

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

private:
    void handle_event(sf::Event& _event) const;
    bool connect();
    void handlePacket(sf::Packet& packet) const;

    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<TronGame> tron_game;
    std::unique_ptr<InputHandler> input_handler;

    sf::IpAddress ip_address;
    unsigned int tcp_port;
    sf::TcpSocket socket;
    sf::Socket::Status socket_status;
    bool exit;
    std::string user_name;

};
