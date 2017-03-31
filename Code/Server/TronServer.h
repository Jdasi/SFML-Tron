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

    void mainLoop();
    void listen();
    void handleServerReset();
    void performStateBehaviour(const double _dt);

    void acceptClient();
    int generateUniqueID() const;
    void sendClientIdentity(const ClientPtr& _client) const;
    void sendClientList(const ClientPtr& _client);
    void sendClientJoined(const ClientPtr& _client);
    void sendUpdatedClientState(const ClientPtr& _client);
    void receivePacket();

    void handlePacket(sf::Packet& _packet, ClientPtr& _sender);
    void handleDisconnectPacket(sf::Packet& _packet, ClientPtr& _sender);
    void handlePingPacket(sf::Packet& _packet, ClientPtr& _sender);
    void handleLatencyPacket(sf::Packet& _packet, ClientPtr& _sender);
    void handleMessagePacket(sf::Packet& _packet, ClientPtr& _sender);
    void handlePlayerStatePacket(const sf::Packet& _packet, ClientPtr& _sender);
    void handleDirectionPacket(sf::Packet& _packet, ClientPtr& _sender);
    void handleBoostPacket(sf::Packet& _packet, ClientPtr& _sender);

    void disconnectClient(ClientPtr& _client);

    void sendPacketToClient(sf::Packet& _packet, const ClientPtr& _client) const;
    void sendPacketToAll(sf::Packet& _packet);
    void sendPacketToAllButSender(sf::Packet& _packet, const ClientPtr& _sender);

    void startSimulation();
    void stopSimulation();

    void syncBike(unsigned int _bike_id);
    void syncAllBikes();
    void syncSimulation();

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
    bool bike_sync_needed;
    bool full_sync_needed;

};
