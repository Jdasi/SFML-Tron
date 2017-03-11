#pragma once
#include "NetworkManager.h"

class INetworkClient;

class TronNetworkManager final : public NetworkManager
{
public:
    explicit TronNetworkManager(INetworkClient& _client, const sf::IpAddress _ip_address, const unsigned int _tcp_port);
    ~TronNetworkManager() = default;

private:
    void registerGamePacketHandlers();

    void handleMessagePacket(sf::Packet& _packet);

    void updatePingTime(const sf::Uint32 ping) override;

    INetworkClient& client;
};
