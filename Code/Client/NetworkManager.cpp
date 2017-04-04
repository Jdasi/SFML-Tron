#include <functional>
#include <iostream>

#include "NetworkManager.h"

#include <Game/PlayerState.h>
#include <Game/Simulation.h>
#include <Game/FlowControl.h>
#include "INetworkClient.h"
#include "Player.h"

using namespace std::placeholders;

#define registerPacketHandler(id, func) \
    packet_handlers.emplace(id, std::bind(&NetworkManager::func, this, _1))

NetworkManager::NetworkManager(INetworkClient& _client, const sf::IpAddress _ip_address, 
    const unsigned int _tcp_port)
    : client(_client)
    , packet_handlers()
    , ip_address(_ip_address)
    , tcp_port(_tcp_port)
    , socket()
    , has_connected(false)
    , running(true)
    , scheduler()
{
    registerPacketHandlers();

    network_thread = std::thread([this]()
    {
        std::cout << "Networking thread started." << std::endl;
        networkingThread();
        std::cout << "Networking thread stopping." << std::endl;
    });
}



NetworkManager::~NetworkManager()
{
    stopNetworkingThread();
}



void NetworkManager::connect()
{
    postEvent([this]()
    {
        socket.setBlocking(true);

        auto status = sf::Socket::Disconnected;
        do
        {
            status = socket.connect(ip_address, tcp_port);

        } while (status != sf::Socket::Done && !client.isExiting());

        socket.setBlocking(false);
        has_connected = true;

        client.onConnected();

        sendPing();

        std::cout << "Connected to server: " << ip_address << std::endl;
    });
}



void NetworkManager::disconnect()
{
    postEvent([this]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::DISCONNECT);

        socket.send(packet);
    });
}



void NetworkManager::sendChatMessage(const std::string& _message)
{
    postEvent([this, _message]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::MESSAGE);

        packet << _message;

        sendPacket(packet);
    });
}



void NetworkManager::sendPlayerStateChange(const PlayerState _state)
{
    postEvent([this, _state]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::PLAYER_STATE);

        packet << static_cast<sf::Uint8>(_state);

        sendPacket(packet);
    });
}



void NetworkManager::sendBikeDirectionChange(const MoveDirection _dir)
{
    postEvent([this, _dir]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::BIKE_DIRECTION);

        packet << static_cast<sf::Uint8>(_dir);

        sendPacket(packet);
    });
}



void NetworkManager::sendBikeBoost()
{
    postEvent([this]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::BIKE_BOOST);

        sendPacket(packet);
    });
}



void NetworkManager::networkingThread()
{
    while (running)
    {
        // Ease CPU usage.
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        executeDispatchedMethods();
        scheduler.update();

        if (!has_connected)
        {
            continue;
        }

        sf::Packet packet;
        auto status = socket.receive(packet);

        switch (status)
        {
            case sf::Socket::Done:
            {
                handlePacket(packet);
            } break;

            case sf::Socket::Disconnected:
            case sf::Socket::Error:
            {
                running = false;
                client.onDisconnected();
                return;
            }

            case sf::Socket::NotReady:
            case sf::Socket::Partial:
            default:
            {
            } break;
        }
    }

    std::cout << "Networking thread ended" << std::endl;
}



void NetworkManager::stopNetworkingThread()
{
    running = false;
    network_thread.join();
    std::cout << "Networking thread stopped." << std::endl;
}



void NetworkManager::registerPacketHandlers()
{
    registerPacketHandler(PacketID::PONG,               handlePongPacket);
    registerPacketHandler(PacketID::IDENTITY,           handleIdentityPacket);
    registerPacketHandler(PacketID::PLAYER_LIST,        handlePlayerListPacket);
    registerPacketHandler(PacketID::PLAYER_JOINED,      handlePlayerJoinedPacket);
    registerPacketHandler(PacketID::PLAYER_LEFT,        handlePlayerLeftPacket);
    registerPacketHandler(PacketID::MESSAGE,            handleMessagePacket);
    registerPacketHandler(PacketID::PLAYER_STATE,       handlePlayerStateChangePacket);
    registerPacketHandler(PacketID::GAME_STATE,         handleGameStateChangePacket);
    registerPacketHandler(PacketID::FLOW_CONTROL,       handleFlowControlPacket);
    registerPacketHandler(PacketID::VICTOR,             handleVictorPacket);
    registerPacketHandler(PacketID::SYNC_BIKE,          handleBikeSyncPacket);
    registerPacketHandler(PacketID::SYNC_ALL_BIKES,     handleFullBikeSyncPacket);
    registerPacketHandler(PacketID::SYNC_SIMULATION,    handleFullSyncPacket);
    registerPacketHandler(PacketID::BIKE_REMOVED,       handleBikeRemovedPacket);
    registerPacketHandler(PacketID::BIKE_BOOST,         handleBikeBoostPacket);
    registerPacketHandler(PacketID::EXTRA_BOOST,        handleExtraBoostChargePacket);
}



void NetworkManager::handlePacket(sf::Packet& _packet)
{
    PacketID pid = getPacketID(_packet);
    packet_handlers.at(pid)(_packet);
}



void NetworkManager::handlePongPacket(sf::Packet& _packet)
{
    using namespace std::chrono;

    double latency = static_cast<double>(duration_cast<microseconds>(
        steady_clock::now() - pre_ping).count()) / 2;

    client.onUpdatePingTime(latency);
    sendClientLatency(latency);

    scheduler.invoke([this](){ sendPing(); }, 1.0);
}



void NetworkManager::handleIdentityPacket(sf::Packet& _packet) const
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    client.onIdentity(static_cast<int>(temp_id));
}



void NetworkManager::handlePlayerListPacket(sf::Packet& _packet) const
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

    client.onPlayerList(players);
}



void NetworkManager::handlePlayerJoinedPacket(sf::Packet& _packet) const
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    client.onPlayerJoined(static_cast<unsigned int>(temp_id));
}



void NetworkManager::handlePlayerLeftPacket(sf::Packet& _packet) const
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    client.onPlayerLeft(static_cast<int>(temp_id));
}



void NetworkManager::handleMessagePacket(sf::Packet& _packet) const
{
    std::string str;
    _packet >> str;

    std::cout << str << std::endl;
}



void NetworkManager::handlePlayerStateChangePacket(sf::Packet& _packet) const
{
    sf::Uint8 temp_id;
    sf::Uint8 temp_state;

    _packet >> temp_id >> temp_state;

    client.onPlayerStateChange(temp_id, static_cast<PlayerState>(temp_state));
}



void NetworkManager::handleGameStateChangePacket(sf::Packet& _packet) const
{
    sf::Uint8 state;
    _packet >> state;

    client.onGameStateChange(state);
}



void NetworkManager::handleFlowControlPacket(sf::Packet& _packet) const
{
    sf::Uint8 flow;
    _packet >> flow;

    client.onFlowControl(static_cast<FlowControl>(flow));
}



void NetworkManager::handleVictorPacket(sf::Packet& _packet) const
{
    sf::Uint8 bike_id;
    _packet >> bike_id;

    client.onVictor(bike_id);
}



void NetworkManager::handleBikeSyncPacket(sf::Packet& _packet) const
{
    BikeState bike_state;
    _packet >> bike_state;

    client.onBikeSync(bike_state);
}



void NetworkManager::handleFullBikeSyncPacket(sf::Packet& _packet) const
{
    std::array<BikeState, MAX_PLAYERS> bike_states;

    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        _packet >> bike_states[i];
    }

    client.onFullBikeSync(bike_states);
}



void NetworkManager::handleFullSyncPacket(sf::Packet& _packet) const
{
    SimulationState simulation_state;
    _packet >> simulation_state;

    client.onFullSync(simulation_state);
}



void NetworkManager::handleBikeRemovedPacket(sf::Packet& _packet) const
{
    sf::Uint8 bike_id;
    _packet >> bike_id;

    client.onBikeRemoved(bike_id);
}



void NetworkManager::handleBikeBoostPacket(sf::Packet& _packet) const
{
    sf::Uint8 bike_id;
    _packet >> bike_id;

    client.onBikeBoost(bike_id);
}



void NetworkManager::handleExtraBoostChargePacket(sf::Packet& _packet) const
{
    sf::Uint8 bike_id;
    _packet >> bike_id;

    client.onBoostChargeGranted(bike_id);
}



void NetworkManager::sendPacket(sf::Packet& _packet)
{
    while (socket.send(_packet) == sf::Socket::Partial){}
}



void NetworkManager::sendClientLatency(const double _latency)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::LATENCY);

    packet << _latency;
    
    sendPacket(packet);
}



void NetworkManager::sendPing()
{
    pre_ping = std::chrono::steady_clock::now();

    sf::Packet packet;
    setPacketID(packet, PacketID::PING);

    sendPacket(packet);
}
