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
    void sendPlayerStateChange(int _player_id, PlayerState _state);
    void sendBikeDirectionChange(int _player_id, MoveDirection _dir);

private:
    void registerGamePacketHandlers();

    // Non-generic network packet handlers.
    void handleIdentityPacket(sf::Packet& _packet);
    void handlePlayerListPacket(sf::Packet& _packet);
    void handlePlayerJoinedPacket(sf::Packet& _packet);
    void handleMessagePacket(sf::Packet& _packet) const;
    void handleDirectionPacket(sf::Packet& _packet);
    void handlePlayerStateChangePacket(sf::Packet& _packet);
    void handleGameStateChangePacket(sf::Packet& _packet);
    void handleBikeSyncPacket(sf::Packet& _packet);
    void handleFullSyncPacket(sf::Packet& _packet);

    // Pass-through functions to talk to the client.
    void onConnected() override;
    void onDisconnected() override;
    void onUpdatePingTime(double _ping) override;
    void onBikeDirectionChange(int _id, MoveDirection _dir);
    void onIdentity(int _id);
    void onPlayerList(std::vector<Player> _players);
    void onPlayerJoined(int _id);
    void onPlayerStateChange(int _player_id, PlayerState _state);
    void onGameStateChange(int _state);
    void onBikeSync(Bike& _bike);
    void onFullSync(Simulation& _simulation);

    // TronClient network interface.
    INetworkClient& client;
};
