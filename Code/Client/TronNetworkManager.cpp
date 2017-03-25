#include <functional>
#include <iostream>

#include "TronNetworkManager.h"
#include "NetworkClient.h"

using namespace std::placeholders;

TronNetworkManager::TronNetworkManager(INetworkClient& _client, const sf::IpAddress _ip_address, const unsigned _tcp_port)
    : NetworkManager(_ip_address, _tcp_port)
    , client(_client)
{
    registerGamePacketHandlers();
}

void TronNetworkManager::sendChatMessage(const std::string& _message)
{
    postEvent([this, _message]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::MESSAGE);

        packet << _message;

        sendPacket(packet);
    });
}

void TronNetworkManager::sendPlayerDirectionChange(int _id, MoveDirection _dir)
{
    postEvent([this, _id, _dir]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::DIRECTION);

        packet << static_cast<sf::Uint8>(_id) << static_cast<sf::Uint8>(_dir);

        sendPacket(packet);
    });
}

void TronNetworkManager::registerGamePacketHandlers()
{
    registerPacketHandler(MESSAGE, std::bind(&TronNetworkManager::handleMessagePacket, this, _1));
    registerPacketHandler(DIRECTION, std::bind(&TronNetworkManager::handleDirectionPacket, this, _1));
}

void TronNetworkManager::handleMessagePacket(sf::Packet& _packet)
{
    std::string str;
    _packet >> str;
    std::cout << str << std::endl;
}

void TronNetworkManager::handleDirectionPacket(sf::Packet& _packet)
{
    sf::Uint8 val1;
    sf::Uint8 val2;

    _packet >> val1 >> val2;

    int id = static_cast<int>(val1);
    MoveDirection dir = static_cast<MoveDirection>(val2);

    onPlayerDirectionChange(id, dir);
}

void TronNetworkManager::onConnected()
{
    client.onConnected();
}

void TronNetworkManager::onDisconnected()
{
    client.onDisconnected();
}

void TronNetworkManager::onUpdatePingTime(const sf::Uint32 _ping)
{
    client.onUpdatePingTime(_ping);
}

void TronNetworkManager::onPlayerDirectionChange(int _id, MoveDirection _dir)
{
    client.onPlayerDirectionChange(_id, _dir);
}
