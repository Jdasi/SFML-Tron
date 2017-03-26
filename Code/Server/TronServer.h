#pragma once
#include <memory>
#include <vector>

#include <SFML/Network.hpp>

#include <Game/Simulation.h>
#include <Game/Scheduler.h>
#include "Client.h"

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
    int generateUniqueID() const;
    void sendClientIdentity(std::unique_ptr<Client>& _client) const;
    void sendClientList(std::unique_ptr<Client>& _client);
    void sendClientJoined(std::unique_ptr<Client>& _client);
    void receivePacket();
    void handlePacket(sf::Packet& _packet, std::unique_ptr<Client>& _sender);
    void handleDisconnect(std::unique_ptr<Client>& _client);

    void sendPacketToAll(sf::Packet& _packet);
    void sendPacketToAllButSender(sf::Packet& _packet, std::unique_ptr<Client>& _sender);

    unsigned int tcp_port;
    sf::TcpListener tcp_listener;
    sf::SocketSelector socket_selector;
    std::vector<std::unique_ptr<Client>> clients;
    int connected_clients;

    bool exit;
    std::string server_name;
    std::string welcome_message;

    SimpleTimer simple_timer;
    Scheduler scheduler;

    int server_state;

};
