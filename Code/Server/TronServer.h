#pragma once
#include <memory>
#include <vector>
#include <map>

#include <SFML/Network.hpp>

#include <Game/PacketID.h>
#include <Game/Simulation.h>
#include <Game/Scheduler.h>
#include "Client.h"

using ClientPtr = std::unique_ptr<Client>;

class TronServer
{
public:
    TronServer();
    ~TronServer() = default;

    bool run();

private:
    void registerPacketHandlers();

    bool bindServerPort();
    void listen();
    void acceptClient();
    int generateUniqueID() const;
    void sendClientIdentity(ClientPtr& _client) const;
    void sendClientList(ClientPtr& _client);
    void sendClientJoined(ClientPtr& _client);
    void receivePacket();

    void handlePacket(sf::Packet& _packet, ClientPtr& _sender);
    void handleDisconnectPacket(sf::Packet& _packet, ClientPtr& _sender);
    void handlePingPacket(sf::Packet& _packet, ClientPtr& _sender);
    void handleLatencyPacket(sf::Packet& _packet, ClientPtr& _sender);
    void handleMessagePacket(sf::Packet& _packet, ClientPtr& _sender);
    void handlePlayerStatePacket(sf::Packet& _packet, ClientPtr& _sender);
    void handleDirectionPacket(sf::Packet& _packet, ClientPtr& _sender);

    void onDisconnect(ClientPtr& _client);

    void sendPacketToAll(sf::Packet& _packet);
    void sendPacketToAllButSender(sf::Packet& _packet, ClientPtr& _sender);

    void syncBike(unsigned int _bike_id);
    void fullSimulationSync();

    sf::TcpListener tcp_listener;
    sf::SocketSelector socket_selector;
    std::vector<ClientPtr> clients;
    int connected_clients;
    std::map<PacketID, std::function<void(sf::Packet&, ClientPtr&)>> packet_handlers;

    bool exit;
    std::string server_name;
    std::string welcome_message;

    Simulation simulation;
    SimpleTimer simple_timer;
    Scheduler scheduler;

    int server_state;
    bool full_sync_needed;

};
