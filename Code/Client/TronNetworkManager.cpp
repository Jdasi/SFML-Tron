#include <functional>
#include <iostream>

#include <Game/PlayerState.h>
#include <Game/Simulation.h>
#include "TronNetworkManager.h"
#include "INetworkClient.h"
#include "Player.h"

using namespace std::placeholders;

#define registerPacketHandler(id, func) \
    packet_handlers.emplace(id, std::bind(&TronNetworkManager::func, this, _1))

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

void TronNetworkManager::sendPlayerStateChange()
{
    postEvent([this]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::PLAYER_STATE);

        sendPacket(packet);
    });
}

void TronNetworkManager::sendBikeDirectionChange(const MoveDirection _dir)
{
    postEvent([this, _dir]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::DIRECTION);

        packet << static_cast<sf::Uint8>(_dir);

        sendPacket(packet);
    });
}

void TronNetworkManager::sendBikeBoost()
{
    postEvent([this]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::BOOST);

        sendPacket(packet);
    });
}

void TronNetworkManager::registerGamePacketHandlers()
{
    registerPacketHandler(IDENTITY,         handleIdentityPacket);
    registerPacketHandler(PLAYER_LIST,      handlePlayerListPacket);
    registerPacketHandler(PLAYER_JOINED,    handlePlayerJoinedPacket);
    registerPacketHandler(MESSAGE,          handleMessagePacket);
    registerPacketHandler(PLAYER_STATE,     handlePlayerStateChangePacket);
    registerPacketHandler(GAME_STATE,       handleGameStateChangePacket);
    registerPacketHandler(SYNC_BIKE,        handleBikeSyncPacket);
    registerPacketHandler(SYNC_ALL_BIKES,   handleFullBikeSyncPacket);
    registerPacketHandler(SYNC_SIMULATION,  handleFullSyncPacket);
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

void TronNetworkManager::handleFullBikeSyncPacket(sf::Packet& _packet)
{
    std::array<Bike, MAX_PLAYERS> bikes;

    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        _packet >> bikes[i];
    }

    onFullBikeSync(bikes);
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

void TronNetworkManager::onPlayerList(const std::vector<Player>& _players)
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

void TronNetworkManager::onBikeSync(const Bike& _bike)
{
    client.onBikeSync(_bike);
}

void TronNetworkManager::onFullBikeSync(const std::array<Bike, MAX_PLAYERS>& _bikes)
{
    client.onFullBikeSync(_bikes);
}

void TronNetworkManager::onFullSync(const Simulation& _simulation)
{
    client.onFullSync(_simulation);
}
