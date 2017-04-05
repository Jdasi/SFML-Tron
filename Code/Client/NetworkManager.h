#pragma once
#include <map>
#include <functional>
#include <thread>
#include <chrono>

#include <SFML/Network.hpp>

#include <Game/PacketID.h>
#include <Game/Scheduler.h>
#include <Game/ThreadDispatcher.h>
#include <Game/MoveDirection.h>
#include <Game/PlayerState.h>

class INetworkClient;
class Player;
struct SimulationState;
struct BikeState;

/* Class for handling all of the network messages from the client to server
 * and vice versa.
 *
 * NetworkManager owns the thread on which all network events are handled.
 * TronClient and NetworkManager post events to each other to send data
 * over the thread boundary, minimising the amount of disruption on either thread.
 */
class NetworkManager final : public ThreadDispatcher
{
public:
    explicit NetworkManager(INetworkClient& _client);
    ~NetworkManager();

    void connect(const sf::IpAddress& _ip_address, const unsigned int _tcp_port);
    void disconnect();

    // Network events for the TronClient to call.
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
    void handleExtraBoostChargePacket(sf::Packet& _packet) const;
    void handleServerBulletinPacket(sf::Packet& _packet) const;

    // Convenience methods.
    void sendPacket(sf::Packet& _packet);
    void sendPing();

    // TronClient network interface.
    INetworkClient& client;
    std::thread network_thread;
    sf::TcpSocket socket;

    std::map<PacketID, std::function<void(sf::Packet&)>> packet_handlers;

    volatile bool has_connected;
    volatile bool running;

    std::chrono::steady_clock::time_point pre_ping;
    Scheduler scheduler;

};
