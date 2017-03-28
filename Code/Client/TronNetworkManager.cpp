#include <functional>
#include <iostream>

#include <Game/PlayerState.h>
#include <Game/Simulation.h>
#include "TronNetworkManager.h"
#include "INetworkClient.h"
#include "Player.h"

using namespace std::placeholders;

TronNetworkManager::TronNetworkManager(INetworkClient& _client, 
    const sf::IpAddress _ip_address, const unsigned int _tcp_port)
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

void TronNetworkManager::sendPlayerStateChange(int _player_id, PlayerState _state)
{
    postEvent([this, _player_id, _state]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::PLAYER_STATE);

        packet << static_cast<sf::Uint8>(_player_id) << static_cast<sf::Uint8>(_state);

        sendPacket(packet);
    });
}

void TronNetworkManager::sendBikeDirectionChange(int _player_id, MoveDirection _dir)
{
    postEvent([this, _player_id, _dir]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::DIRECTION);

        packet << static_cast<sf::Uint8>(_player_id) << static_cast<sf::Uint8>(_dir);

        sendPacket(packet);
    });
}

void TronNetworkManager::registerGamePacketHandlers()
{
    registerPacketHandler(IDENTITY, std::bind(&TronNetworkManager::handleIdentityPacket, this, _1));
    registerPacketHandler(PLAYER_LIST, std::bind(&TronNetworkManager::handlePlayerListPacket, this, _1));
    registerPacketHandler(PLAYER_JOINED, std::bind(&TronNetworkManager::handlePlayerJoinedPacket, this, _1));
    registerPacketHandler(MESSAGE, std::bind(&TronNetworkManager::handleMessagePacket, this, _1));
    registerPacketHandler(DIRECTION, std::bind(&TronNetworkManager::handleDirectionPacket, this, _1));
    registerPacketHandler(PLAYER_STATE, std::bind(&TronNetworkManager::handlePlayerStateChangePacket, this, _1));
    registerPacketHandler(GAME_STATE, std::bind(&TronNetworkManager::handleGameStateChangePacket, this, _1));
    registerPacketHandler(SYNC_BIKE, std::bind(&TronNetworkManager::handleBikeSyncPacket, this, _1));
    registerPacketHandler(SYNC_SIMULATION, std::bind(&TronNetworkManager::handleFullSyncPacket, this, _1));
}

void TronNetworkManager::handleIdentityPacket(sf::Packet& _packet)
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    onIdentity(static_cast<int>(temp_id));
}

void TronNetworkManager::handlePlayerListPacket(sf::Packet& _packet)
{
    std::vector<Player> players;

    while (!_packet.endOfPacket())
    {
        sf::Uint8 id;
        sf::Uint8 state;
        
        _packet >> id >> state;

        Player player(id);
        player.setState(static_cast<PlayerState>(state));

        players.push_back(player);
    }

    onPlayerList(players);
}

void TronNetworkManager::handlePlayerJoinedPacket(sf::Packet& _packet)
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    onPlayerJoined(static_cast<int>(temp_id));
}

void TronNetworkManager::handleMessagePacket(sf::Packet& _packet) const
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

void TronNetworkManager::handlePlayerStateChangePacket(sf::Packet& _packet)
{
    sf::Uint8 temp_id;
    sf::Uint8 temp_state;

    _packet >> temp_id >> temp_state;

    onPlayerStateChange(temp_id, static_cast<PlayerState>(temp_state));
}

void TronNetworkManager::handleGameStateChangePacket(sf::Packet& _packet)
{
    sf::Uint8 state;

    _packet >> state;

    onGameStateChange(state);
}

void TronNetworkManager::handleBikeSyncPacket(sf::Packet& _packet)
{
    Bike bike;
    _packet >> bike;

    onBikeSync(bike);
}

void TronNetworkManager::handleFullSyncPacket(sf::Packet& _packet)
{
    Simulation sim;
    _packet >> sim;

    onFullSync(sim);
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

void TronNetworkManager::onPlayerList(std::vector<Player> _players)
{
    client.onPlayerList(_players);
}

void TronNetworkManager::onIdentity(int _id)
{
    client.onIdentity(_id);
}

void TronNetworkManager::onPlayerJoined(int _id)
{
    client.onPlayerJoined(_id);
}

void TronNetworkManager::onPlayerStateChange(int _player_id, PlayerState _state)
{
    client.onPlayerStateChange(_player_id, _state);
}

void TronNetworkManager::onGameStateChange(int _state)
{
    client.onGameStateChange(_state);
}

void TronNetworkManager::onBikeSync(Bike& _bike)
{
    client.onBikeSync(_bike);
}

void TronNetworkManager::onFullSync(Simulation& _simulation)
{
    client.onFullSync(_simulation);
}
