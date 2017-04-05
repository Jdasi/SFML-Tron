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

// Initialises data and starts up the network thread.
NetworkManager::NetworkManager(INetworkClient& _client)
    : client(_client)
    , socket()
    , packet_handlers()
    , has_connected(false)
    , running(true)
    , scheduler()
{
    registerPacketHandlers();

    network_thread = std::thread([this]()
    {
        networkingThread();
    });
}



// The thread must be joined upon destruction of the NetworkManager.
NetworkManager::~NetworkManager()
{
    stopNetworkingThread();
}



// Continually tries to connect to the server until the program exits.
void NetworkManager::connect(const sf::IpAddress& _ip_address,
    const unsigned int _tcp_port)
{
    postEvent([this, _ip_address, _tcp_port]()
    {
        socket.setBlocking(true);

        auto status = sf::Socket::Disconnected;
        do
        {
            status = socket.connect(_ip_address, _tcp_port);

        } while (status != sf::Socket::Done && !client.isExiting());

        socket.setBlocking(false);
        has_connected = true;

        // Inform the client we have connected.
        client.onConnected();

        sendPing();
    });
}



// Inform the server when the client gracefully disconnects.
void NetworkManager::disconnect()
{
    postEvent([this]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::DISCONNECT);

        socket.send(packet);
    });
}



// Call to send a message to the server.
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



/* Call to send a PlayerState associated with the client's id to the server.
 * The server already knows the client's id, so it does not need to be sent.
 */
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



/* Call to change the direction of the bike associated with the client's id.
 * The server already knows the client's id, so it does not need to be sent.
 */
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



/* Call to activate the boost of the bike associated with the client's id.
 * The server already knows the client's id, so it does not need to be sent.
 */
void NetworkManager::sendBikeBoost()
{
    postEvent([this]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::BIKE_BOOST);

        sendPacket(packet);
    });
}



/* NetworkManager's verson of a 'main loop', on its own thread.
 * Packets are listened for here and are passed on for handling.
 */
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

                break;
            }

            case sf::Socket::Disconnected:
            case sf::Socket::Error:
            {
                running = false;
                client.onDisconnected();

                break;
            }

            case sf::Socket::NotReady:
            case sf::Socket::Partial:
            default: {}
        }
    }
}



void NetworkManager::stopNetworkingThread()
{
    running = false;
    network_thread.join();
}



// The packet handlers required by the client to handle incoming server messages.
void NetworkManager::registerPacketHandlers()
{
    registerPacketHandler(PacketID::PONG,             handlePongPacket);
    registerPacketHandler(PacketID::IDENTITY,         handleIdentityPacket);
    registerPacketHandler(PacketID::PLAYER_LIST,      handlePlayerListPacket);
    registerPacketHandler(PacketID::PLAYER_JOINED,    handlePlayerJoinedPacket);
    registerPacketHandler(PacketID::PLAYER_LEFT,      handlePlayerLeftPacket);
    registerPacketHandler(PacketID::MESSAGE,          handleMessagePacket);
    registerPacketHandler(PacketID::BULLETIN,         handleServerBulletinPacket);
    registerPacketHandler(PacketID::PLAYER_STATE,     handlePlayerStateChangePacket);
    registerPacketHandler(PacketID::GAME_STATE,       handleGameStateChangePacket);
    registerPacketHandler(PacketID::FLOW_CONTROL,     handleFlowControlPacket);
    registerPacketHandler(PacketID::VICTOR,           handleVictorPacket);
    registerPacketHandler(PacketID::SYNC_BIKE,        handleBikeSyncPacket);
    registerPacketHandler(PacketID::SYNC_ALL_BIKES,   handleFullBikeSyncPacket);
    registerPacketHandler(PacketID::SYNC_SIMULATION,  handleFullSyncPacket);
    registerPacketHandler(PacketID::BIKE_REMOVED,     handleBikeRemovedPacket);
    registerPacketHandler(PacketID::BIKE_BOOST,       handleBikeBoostPacket);
    registerPacketHandler(PacketID::EXTRA_BOOST,      handleExtraBoostChargePacket);
}



/* Generic pass-through function which determines the appropriate 
 * packet handler function to call, based on those which have been registered.
 */
void NetworkManager::handlePacket(sf::Packet& _packet)
{
    PacketID pid = getPacketID(_packet);
    packet_handlers.at(pid)(_packet);
}



/* Client-side ping-pong behaviour.
 * Once a pong has been sent and received, a new ping request is scheduled.
 */
void NetworkManager::handlePongPacket(sf::Packet& _packet)
{
    using namespace std::chrono;

    double latency = static_cast<double>(duration_cast<microseconds>(
        steady_clock::now() - pre_ping).count()) / 2;

    client.onUpdatePingTime(latency);

    scheduler.invoke([this](){ sendPing(); }, 1.0);
}



// Informs the client's id which is used for bike controls and colouring.
void NetworkManager::handleIdentityPacket(sf::Packet& _packet) const
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    client.onIdentity(static_cast<int>(temp_id));
}



// Informs the client of all players currently connected to the server.
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



// Informs the client when a new player has joined the server.
void NetworkManager::handlePlayerJoinedPacket(sf::Packet& _packet) const
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    client.onPlayerJoined(static_cast<unsigned int>(temp_id));
}



// Informs the client when a player has left the server.
void NetworkManager::handlePlayerLeftPacket(sf::Packet& _packet) const
{
    sf::Uint8 temp_id;
    _packet >> temp_id;

    client.onPlayerLeft(static_cast<int>(temp_id));
}



void NetworkManager::handleMessagePacket(sf::Packet& _packet) const
{
    std::string message;
    _packet >> message;

    std::cout << message << std::endl;
}



// Informs the client when the server's bulletin changes.
void NetworkManager::handleServerBulletinPacket(sf::Packet& _packet) const
{
    std::string bulletin;
    _packet >> bulletin;

    client.onBulletinUpdate(bulletin);
}



// Informs the client when a user's state changes.
void NetworkManager::handlePlayerStateChangePacket(sf::Packet& _packet) const
{
    sf::Uint8 temp_id;
    sf::Uint8 temp_state;

    _packet >> temp_id >> temp_state;

    client.onPlayerStateChange(temp_id, static_cast<PlayerState>(temp_state));
}



// Informs the client when the server's state changes.
void NetworkManager::handleGameStateChangePacket(sf::Packet& _packet) const
{
    sf::Uint8 state;
    _packet >> state;

    client.onGameStateChange(state);
}



// Informs the client what state the server's simulation is in.
void NetworkManager::handleFlowControlPacket(sf::Packet& _packet) const
{
    sf::Uint8 flow;
    _packet >> flow;

    client.onFlowControl(static_cast<FlowControl>(flow));
}



// Informs the client who won the last round.
void NetworkManager::handleVictorPacket(sf::Packet& _packet) const
{
    sf::Uint8 bike_id;
    _packet >> bike_id;

    client.onVictor(bike_id);
}



// Informs the client of a bike that needs to be synchronised.
void NetworkManager::handleBikeSyncPacket(sf::Packet& _packet) const
{
    BikeState bike_state;
    _packet >> bike_state;

    client.onBikeSync(bike_state);
}



// Informs the client that all bikes need to be synchronised.
void NetworkManager::handleFullBikeSyncPacket(sf::Packet& _packet) const
{
    std::array<BikeState, MAX_PLAYERS> bike_states;

    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        _packet >> bike_states[i];
    }

    client.onFullBikeSync(bike_states);
}



// Informs the client that the whole simulation needs to be synchronised.
void NetworkManager::handleFullSyncPacket(sf::Packet& _packet) const
{
    SimulationState simulation_state;
    _packet >> simulation_state;

    client.onFullSync(simulation_state);
}



/* Informs the client that a bike was removed from the simulation.
 * Either because the controlling player left or the bike was destroyed.
 */
void NetworkManager::handleBikeRemovedPacket(sf::Packet& _packet) const
{
    sf::Uint8 bike_id;
    _packet >> bike_id;

    client.onBikeRemoved(bike_id);
}



// Informs the client a bike activated its boost.
void NetworkManager::handleBikeBoostPacket(sf::Packet& _packet) const
{
    sf::Uint8 bike_id;
    _packet >> bike_id;

    client.onBikeBoost(bike_id);
}



// Informs the client a bike was given an additional boost charge.
void NetworkManager::handleExtraBoostChargePacket(sf::Packet& _packet) const
{
    sf::Uint8 bike_id;
    _packet >> bike_id;

    client.onBoostChargeGranted(bike_id);
}



// Convenience method for sending whole packets in non-blocking mode.
void NetworkManager::sendPacket(sf::Packet& _packet)
{
    while (socket.send(_packet) == sf::Socket::Partial){}
}



// Convenience method for sending pings to the server.
void NetworkManager::sendPing()
{
    pre_ping = std::chrono::steady_clock::now();

    sf::Packet packet;
    setPacketID(packet, PacketID::PING);

    sendPacket(packet);
}
