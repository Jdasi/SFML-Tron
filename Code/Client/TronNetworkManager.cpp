#include "TronNetworkManager.h"
#include "NetworkClient.h"

#include <functional>
#include <iostream>

using namespace std::placeholders;

TronNetworkManager::TronNetworkManager(INetworkClient& _client, const sf::IpAddress _ip_address, const unsigned _tcp_port)
    : NetworkManager(_ip_address, _tcp_port)
    , client(_client)
{
    registerGamePacketHandlers();
}

void TronNetworkManager::registerGamePacketHandlers()
{
    registerPacketHandler(MESSAGE, std::bind(&TronNetworkManager::handleMessagePacket, this, _1));
}

void TronNetworkManager::handleMessagePacket(sf::Packet& _packet)
{
    std::string str;
    _packet >> str;
    std::cout << str << std::endl;
}

void TronNetworkManager::updatePingTime(const sf::Uint32 ping)
{
    client.updatePingTime(ping);
}
