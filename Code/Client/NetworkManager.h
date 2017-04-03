#pragma once
#include <map>
#include <functional>
#include <thread>

#include <SFML/Network.hpp>

#include <Game/Constants.h>
#include <Game/PacketID.h>
#include <Game/Scheduler.h>
#include <Game/SimpleTimer.h>
#include <Game/ThreadDispatcher.h>
#include <Game/MoveDirection.h>
#include <Game/PlayerState.h>

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
    void sendPlayerStateChange();
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
    void handleBikeSyncPacket(sf::Packet& _packet) const;
    void handleFullBikeSyncPacket(sf::Packet& _packet) const;
    void handleFullSyncPacket(sf::Packet& _packet) const;
    void handleBikeBoostPacket(sf::Packet& _packet) const;

    void sendPacket(sf::Packet& _packet);
    void sendClientLatency();
    void sendPing();
    void calculatePlayTime();

    // Pass-through functions to talk to the client.
    void onConnected() const;
    void onDisconnected() const;
    void onUpdatePingTime(const double _ping) const;
    void onIdentity(const unsigned int _id) const;
    void onPlayerList(const std::vector<Player>& _players) const;
    void onPlayerJoined(const unsigned int _player_id) const;
    void onPlayerLeft(const unsigned int _player_id) const;
    void onPlayerStateChange(const unsigned int _player_id, const PlayerState _state) const;
    void onGameStateChange(const int _state) const;
    void onBikeSync(const BikeState& _bike_state) const;
    void onFullBikeSync(const std::array<BikeState, MAX_PLAYERS>& _bike_states) const;
    void onFullSync(const SimulationState& _simulation_state) const;
    void onBikeBoost(const unsigned int _bike_id) const;

    // TronClient network interface.
    INetworkClient& client;
    std::thread network_thread;

    std::map<PacketID, std::function<void(sf::Packet&)>> packet_handlers;

    sf::IpAddress ip_address;
    unsigned int tcp_port;
    sf::TcpSocket socket;

    volatile bool has_connected;
    volatile bool running;
    double latency;
    double play_time;

    SimpleTimer timer;
    Scheduler scheduler;

};
