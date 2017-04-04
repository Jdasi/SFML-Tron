#pragma once
#include <memory>
#include <vector>
#include <map>

#include <SFML/Network.hpp>

#include <Game/PacketID.h>
#include <Game/Scheduler.h>
#include <Game/FlowControl.h>
#include "Client.h"
#include "IServerSimulation.h"
#include "SimulationThread.h"

using ClientPtr = std::unique_ptr<Client>;

class TronServer : public IServerSimulation, public ThreadDispatcher
{
public:
    TronServer();
    ~TronServer() = default;

    bool run(const unsigned int _tcp_port);

private:
    void registerPacketHandlers();
    bool bindServerPort(const unsigned int _tcp_port);

    void mainLoop();
    void listen();
    void handleServerReset();

    void performStateBehaviour();
    void lobbyStateBehaviour();
    void gameStateBehaviour();
    void endStateBehaviour();
    bool clientExistsWithState(const PlayerState _state);

    void acceptClient();
    void receivePacket();
    int generateUniqueClientID() const;
    std::vector<int> getIDsInUse() const;

    void sendClientIdentity(ClientPtr& _client);
    void sendClientBulletin(ClientPtr& _client);
    void sendClientList(ClientPtr& _client);
    void sendClientJoined(const ClientPtr& _client);
    void sendClientLeft(ClientPtr& _client);
    void sendUpdatedClientState(const ClientPtr& _client);
    void sendUpdatedServerState();
    void sendUpdatedFlowControl(const FlowControl _control);
    void sendUpdatedServerBulletin();

    void handlePacket(sf::Packet& _packet, ClientPtr& _sender);
    void handleDisconnectPacket(const sf::Packet& _packet, ClientPtr& _sender);
    void handlePingPacket(const sf::Packet& _packet, ClientPtr& _sender);
    void handleLatencyPacket(sf::Packet& _packet, ClientPtr& _sender) const;
    void handleMessagePacket(sf::Packet& _packet, ClientPtr& _sender);
    void handlePlayerStatePacket(sf::Packet& _packet, ClientPtr& _sender);
    void handleDirectionPacket(sf::Packet& _packet, ClientPtr& _sender);
    void handleBoostPacket(const sf::Packet& _packet, ClientPtr& _sender);

    void disconnectClient(ClientPtr& _client);
    void sendPacketToClient(sf::Packet& _packet, ClientPtr& _client);
    void sendPacketToAll(sf::Packet& _packet);
    void sendPacketToAllButSender(sf::Packet& _packet, const ClientPtr& _sender);
    void sendPacketToAllPlaying(sf::Packet& _packet);

    void onSyncSimulation(const SimulationState& _simulation_state) override;
    void onSyncBike(const BikeState& _bike_state) override;
    void onSyncAllBikes(const std::array<BikeState, MAX_PLAYERS>& _bike_states) override;
    void onBikeRemoved(const unsigned int _bike_id) override;
    void onBikeBoost(const unsigned int _bike_id) override;
    void onBoostChargeGranted(const unsigned int _bike_id) override;
    void onSimulationStarted() override;
    void onSimulationStopping() override;
    void onSimulationEnded() override;
    void onSimulationReset() override;
    void onSimulationVictor(const unsigned int _bike_id) override;

    sf::TcpListener tcp_listener;
    sf::SocketSelector socket_selector;
    std::vector<ClientPtr> clients;
    int connected_clients;

    std::map<PacketID, std::function<void(sf::Packet&, ClientPtr&)>> packet_handlers;

    bool exit;
    std::string server_name;
    std::string welcome_message;
    int server_state;

    SimulationThread simulation_thread;
    Scheduler scheduler;

};
