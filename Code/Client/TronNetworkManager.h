#pragma once
#include "NetworkManager.h"

class INetworkClient;
enum MoveDirection;

class TronNetworkManager final : public NetworkManager
{
public:
    explicit TronNetworkManager(INetworkClient& _client, const sf::IpAddress _ip_address, 
        const unsigned int _tcp_port);
    ~TronNetworkManager() = default;

    void sendChatMessage(const std::string& _message);
    void sendBikeDirectionChange(int _id, MoveDirection _dir);

private:
    void registerGamePacketHandlers();

    // Non-generic network packet handlers.
    void handleIdentityPacket(sf::Packet& _packet);
    void handlePlayerJoinedPacket(sf::Packet& _packet);
    void handleMessagePacket(sf::Packet& _packet);
    void handleDirectionPacket(sf::Packet& _packet);

    // Pass-through functions to talk to the client.
    void onConnected() override;
    void onDisconnected() override;
    void onUpdatePingTime(double _ping) override;
    void onBikeDirectionChange(int _id, MoveDirection _dir) override;
    void onIdentity(int _id) override;
    void onPlayerJoined(int _id) override;

    INetworkClient& client;
};
