#pragma once
#include "NetworkManager.h"
#include <Game/PlayerState.h>
#include <Game/MoveDirection.h>

class Simulation;
class Bike;
class INetworkClient;
class Player;

class TronNetworkManager final : public NetworkManager
{
public:
    explicit TronNetworkManager(INetworkClient& _client, const sf::IpAddress _ip_address, 
        const unsigned int _tcp_port);
    ~TronNetworkManager() = default;

    void sendChatMessage(const std::string& _message);
    void sendPlayerStateChange();
    void sendBikeDirectionChange(const MoveDirection _dir);
    void sendBikeBoost();

private:
    void registerGamePacketHandlers();

    // Non-generic network packet handlers.
    void handleIdentityPacket(sf::Packet& _packet);
    void handlePlayerListPacket(sf::Packet& _packet);
    void handlePlayerJoinedPacket(sf::Packet& _packet);
    void handleMessagePacket(sf::Packet& _packet) const;
    void handlePlayerStateChangePacket(sf::Packet& _packet);
    void handleGameStateChangePacket(sf::Packet& _packet);
    void handleBikeSyncPacket(sf::Packet& _packet);
    void handleFullBikeSyncPacket(sf::Packet& _packet);
    void handleFullSyncPacket(sf::Packet& _packet);

    // Pass-through functions to talk to the client.
    void onConnected() override;
    void onDisconnected() override;
    void onUpdatePingTime(double _ping) override;
    void onIdentity(int _id);
    void onPlayerList(const std::vector<Player>& _players);
    void onPlayerJoined(int _id);
    void onPlayerStateChange(int _player_id, const PlayerState _state);
    void onGameStateChange(int _state);
    void onBikeSync(const Bike& _bike);
    void onFullBikeSync(const std::array<Bike, MAX_PLAYERS>& _bike);
    void onFullSync(const Simulation& _simulation);

    // TronClient network interface.
    INetworkClient& client;
};
