#pragma once
#include <map>
#include <functional>
#include <thread>
#include <chrono>

#include <SFML/Network.hpp>

#include <Game/Constants.h>
#include <Game/PacketID.h>
#include <Game/Scheduler.h>
#include <Game/ThreadDispatcher.h>
#include <Game/MoveDirection.h>
#include <Game/PlayerState.h>

enum class FlowControl;
class INetworkClient;
class Player;
struct SimulationState;
struct BikeState;

class NetworkManager final : public ThreadDispatcher
{
public:
    NetworkManager(INetworkClient& _client, const sf::IpAddress _ip_address,
        const unsigned int _tcp_port);
    ~NetworkManager();

    void connect();
    void disconnect();

    void sendChatMessage(const std::string& _message);
    void sendPlayerStateChange(const PlayerState _state = PlayerState::READY);
    void sendBikeDirectionChange(const MoveDirection _dir);
    void sendBikeBoost();

private:
    void networkingThread();
    void stopNetworkingThread();
    void registerPacketHandlers();

    // Packet handlers.
    void handlePacket(sf::Packet& _packet);
    void handlePongPacket(sf::Packet& _packet);
    void handleIdentityPacket(sf::Packet& _packet) const;
    void handlePlayerListPacket(sf::Packet& _packet) const;
    void handlePlayerJoinedPacket(sf::Packet& _packet) const;
    void handlePlayerLeftPacket(sf::Packet& _packet) const;
    void handleMessagePacket(sf::Packet& _packet) const;
    void handlePlayerStateChangePacket(sf::Packet& _packet) const;
    void handleGameStateChangePacket(sf::Packet& _packet) const;
    void handleFlowControlPacket(sf::Packet& _packet) const;
    void handleVictorPacket(sf::Packet& _packet) const;
    void handleBikeSyncPacket(sf::Packet& _packet) const;
    void handleFullBikeSyncPacket(sf::Packet& _packet) const;
    void handleFullSyncPacket(sf::Packet& _packet) const;
    void handleBikeRemovedPacket(sf::Packet& _packet) const;
    void handleBikeBoostPacket(sf::Packet& _packet) const;

    void sendPacket(sf::Packet& _packet);
    void sendClientLatency(const double _latency);
    void sendPing();

    // TronClient network interface.
    INetworkClient& client;
    std::thread network_thread;

    std::map<PacketID, std::function<void(sf::Packet&)>> packet_handlers;

    sf::IpAddress ip_address;
    unsigned int tcp_port;
    sf::TcpSocket socket;

    volatile bool has_connected;
    volatile bool running;

    std::chrono::steady_clock::time_point pre_ping;
    Scheduler scheduler;

};
