#pragma once
#include <memory>
#include <vector>

#include <SFML/Network.hpp>

#include <Game/Simulation.h>
#include <Game/Scheduler.h>
#include "User.h"

class TronServer
{
public:
    TronServer();
    ~TronServer() = default;

    bool run(unsigned int _port);
    void stop();

private:
    bool bindServerPort();
    void listen();
    void acceptClient();
    void receivePacket();
    void handlePacket(sf::Packet& _packet, std::unique_ptr<User>& _sender);

    void garbageCollectClients();
    void handleDisconnect(std::unique_ptr<User>& _user);

    unsigned int tcp_port;
    sf::TcpListener tcp_listener;
    sf::SocketSelector socket_selector;
    std::vector<std::unique_ptr<User>> users;

    bool exit;
    unsigned int connected_clients;
    bool clients_dirty;
    std::string server_name;
    std::string welcome_message;

    Simulation simulation;
    SimpleTimer simple_timer;
    Scheduler scheduler;

};
