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

void TronNetworkManager::sendBikeDirectionChange(int _id, MoveDirection _dir)
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
    registerPacketHandler(IDENTITY, std::bind(&TronNetworkManager::handleIdentityPacket, this, _1));
    registerPacketHandler(PLAYERJOINED, std::bind(&TronNetworkManager::handlePlayerJoinedPacket, this, _1));
    registerPacketHandler(MESSAGE, std::bind(&TronNetworkManager::handleMessagePacket, this, _1));
    registerPacketHandler(DIRECTION, std::bind(&TronNetworkManager::handleDirectionPacket, this, _1));
}

void TronNetworkManager::handleIdentityPacket(sf::Packet& _packet)
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    onIdentity(static_cast<int>(temp_id));
}

void TronNetworkManager::handlePlayerJoinedPacket(sf::Packet& _packet)
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    onPlayerJoined(static_cast<int>(temp_id));
}

void TronNetworkManager::handleMessagePacket(sf::Packet& _packet)
{
    std::string str;
    _packet >> str;
    std::cout << str << std::endl;
}

void TronNetworkManager::handleDirectionPacket(sf::Packet& _packet)
{
    sf::Uint8 temp_id;
    sf::Uint8 temp_dir;

    _packet >> temp_id >> temp_dir;

    int id = static_cast<int>(temp_id);
    MoveDirection dir = static_cast<MoveDirection>(temp_dir);

    onBikeDirectionChange(id, dir);
}

void TronNetworkManager::onConnected()
{
    client.onConnected();
}

void TronNetworkManager::onDisconnected()
{
    client.onDisconnected();
}

void TronNetworkManager::onUpdatePingTime(double _ping)
{
    client.onUpdatePingTime(_ping);
}

void TronNetworkManager::onBikeDirectionChange(int _id, MoveDirection _dir)
{
    client.onBikeDirectionChange(_id, _dir);
}

void TronNetworkManager::onIdentity(int _id)
{
    client.onIdentity(_id);
}

void TronNetworkManager::onPlayerJoined(int _id)
{
    client.onPlayerJoined(_id);
}
