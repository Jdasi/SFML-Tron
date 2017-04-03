#pragma once
#include <Game/PlayerState.h>
#include <Game/MoveDirection.h>
#include "NetworkManager.h"

class INetworkClient;
class Player;
struct SimulationState;
struct BikeState;

class TronNetworkManager final : public NetworkManager
{
public:
    explicit TronNetworkManager(INetworkClient& _client, const sf::IpAddress _ip_address, 
        const unsigned int _tcp_port);
    ~TronNetworkManager();

    void sendChatMessage(const std::string& _message);
    void sendPlayerStateChange();
    void sendBikeDirectionChange(const MoveDirection _dir);
    void sendBikeBoost();

private:
    void registerGamePacketHandlers();

    // Non-generic network packet handlers.
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

    // Pass-through functions to talk to the client.
    void onConnected() override;
    void onDisconnected() override;
    void onUpdatePingTime(const double _ping) override;
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
};
