#pragma once
#include "NetworkManager.h"

class INetworkClient;

class TronNetworkManager final : public NetworkManager
{
public:
    explicit TronNetworkManager(INetworkClient& _client, const sf::IpAddress _ip_address, const unsigned int _tcp_port);
    ~TronNetworkManager() = default;

    void sendChatMessage(const std::string& _message);

private:
    void registerGamePacketHandlers();

    void handleMessagePacket(sf::Packet& _packet);

    void onConnected() override;
    void onDisconnected() override;
    void onUpdatePingTime(const sf::Uint32 _ping) override;

    INetworkClient& client;
};
