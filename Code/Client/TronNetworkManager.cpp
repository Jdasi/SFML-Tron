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



TronNetworkManager::~TronNetworkManager()
{
    stopNetworkingThread();
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
    registerPacketHandler(PacketID::IDENTITY,         handleIdentityPacket);
    registerPacketHandler(PacketID::PLAYER_LIST,      handlePlayerListPacket);
    registerPacketHandler(PacketID::PLAYER_JOINED,    handlePlayerJoinedPacket);
    registerPacketHandler(PacketID::PLAYER_LEFT,      handlePlayerLeftPacket);
    registerPacketHandler(PacketID::MESSAGE,          handleMessagePacket);
    registerPacketHandler(PacketID::PLAYER_STATE,     handlePlayerStateChangePacket);
    registerPacketHandler(PacketID::GAME_STATE,       handleGameStateChangePacket);
    registerPacketHandler(PacketID::SYNC_BIKE,        handleBikeSyncPacket);
    registerPacketHandler(PacketID::SYNC_ALL_BIKES,   handleFullBikeSyncPacket);
    registerPacketHandler(PacketID::SYNC_SIMULATION,  handleFullSyncPacket);
    registerPacketHandler(PacketID::BOOST,            handleBikeBoostPacket);
}



void TronNetworkManager::handleIdentityPacket(sf::Packet& _packet) const
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    onIdentity(static_cast<int>(temp_id));
}



void TronNetworkManager::handlePlayerListPacket(sf::Packet& _packet) const
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



void TronNetworkManager::handlePlayerJoinedPacket(sf::Packet& _packet) const
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    onPlayerJoined(static_cast<int>(temp_id));
}



void TronNetworkManager::handlePlayerLeftPacket(sf::Packet& _packet) const
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    onPlayerLeft(static_cast<int>(temp_id));
}



void TronNetworkManager::handleMessagePacket(sf::Packet& _packet) const
{
    std::string str;
    _packet >> str;

    std::cout << str << std::endl;
}



void TronNetworkManager::handlePlayerStateChangePacket(sf::Packet& _packet) const
{
    sf::Uint8 temp_id;
    sf::Uint8 temp_state;

    _packet >> temp_id >> temp_state;

    onPlayerStateChange(temp_id, static_cast<PlayerState>(temp_state));
}



void TronNetworkManager::handleGameStateChangePacket(sf::Packet& _packet) const
{
    sf::Uint8 state;
    _packet >> state;

    onGameStateChange(state);
}



void TronNetworkManager::handleBikeSyncPacket(sf::Packet& _packet) const
{
    BikeState bike_state;
    _packet >> bike_state;

    onBikeSync(bike_state);
}



void TronNetworkManager::handleFullBikeSyncPacket(sf::Packet& _packet) const
{
    std::array<BikeState, MAX_PLAYERS> bike_states;

    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        _packet >> bike_states[i];
    }

    onFullBikeSync(bike_states);
}



void TronNetworkManager::handleFullSyncPacket(sf::Packet& _packet) const
{
    SimulationState simulation_state;
    _packet >> simulation_state;

    onFullSync(simulation_state);
}



void TronNetworkManager::handleBikeBoostPacket(sf::Packet& _packet) const
{
    sf::Uint8 bike_id;
    _packet >> bike_id;

    onBikeBoost(bike_id);
}



void TronNetworkManager::onConnected()
{
    client.onConnected();
}



void TronNetworkManager::onDisconnected()
{
    client.onDisconnected();
}



void TronNetworkManager::onUpdatePingTime(const double _ping)
{
    client.onUpdatePingTime(_ping);
}



void TronNetworkManager::onPlayerList(const std::vector<Player>& _players) const
{
    client.onPlayerList(_players);
}



void TronNetworkManager::onIdentity(const unsigned int _player_id) const
{
    client.onIdentity(_player_id);
}



void TronNetworkManager::onPlayerJoined(const unsigned int _player_id) const
{
    client.onPlayerJoined(_player_id);
}



void TronNetworkManager::onPlayerLeft(const unsigned int _player_id) const
{
    client.onPlayerLeft(_player_id);
}



void TronNetworkManager::onPlayerStateChange(const unsigned int _player_id, const PlayerState _state) const
{
    client.onPlayerStateChange(_player_id, _state);
}



void TronNetworkManager::onGameStateChange(const int _state) const
{
    client.onGameStateChange(_state);
}



void TronNetworkManager::onBikeSync(const BikeState& _bike) const
{
    client.onBikeSync(_bike);
}



void TronNetworkManager::onFullBikeSync(const std::array<BikeState, MAX_PLAYERS>& _bike_states) const
{
    client.onFullBikeSync(_bike_states);
}



void TronNetworkManager::onFullSync(const SimulationState& _simulation_state) const
{
    client.onFullSync(_simulation_state);
}



void TronNetworkManager::onBikeBoost(const unsigned int _bike_id) const
{
    client.onBikeBoost(_bike_id);
}
