#include <iostream>

#include <Game/Constants.h>
#include <Game/GameStateIDs.h>
#include "TronServer.h"

#define registerPacketHandler(id, func) \
    packet_handlers.emplace(id, std::bind(&TronServer::func, this, _1, _2))

using namespace std::placeholders;

TronServer::TronServer()
    : connected_clients(0)
    , exit(false)
    , server_name()
    , server_state(STATE_LOBBY)
    , simulation_thread(*this)
{
    registerPacketHandlers();

    clients.reserve(MAX_PLAYERS);
    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        clients.push_back(nullptr);
    }
}



/* Attempts to launch the server on the passed port.
 * Returns true if the server was run successfully, otherwise returns false.
 *
 * If the server launches, this function won't return until after the
 * main loop finishes.
 */
bool TronServer::run(const unsigned int _tcp_port)
{
    if (!bindServerPort(_tcp_port))
    {
        return false;
    }

    std::cout << "Server launched on port: " << _tcp_port << std::endl
              << "> Awaiting clients . . ." << std::endl;

    socket_selector.add(tcp_listener);

    mainLoop();

    return true;
}



void TronServer::registerPacketHandlers()
{
    registerPacketHandler(PacketID::DISCONNECT,      handleDisconnectPacket);
    registerPacketHandler(PacketID::PING,            handlePingPacket);
    registerPacketHandler(PacketID::MESSAGE,         handleMessagePacket);
    registerPacketHandler(PacketID::PLAYER_STATE,    handlePlayerStatePacket);
    registerPacketHandler(PacketID::BIKE_DIRECTION,  handleDirectionPacket);
    registerPacketHandler(PacketID::BIKE_BOOST,      handleBoostPacket);
}



bool TronServer::bindServerPort(const unsigned int _tcp_port)
{
    if (tcp_listener.listen(_tcp_port) != sf::Socket::Done)
    {
        std::cout << "Could not bind server port: " << _tcp_port << std::endl;

        return false;
    }

    return true;
}



/* Executes dispatched methods and performs state behaviour.
 * The server automatically resets if no clients are connected.
 */
void TronServer::mainLoop()
{
    while (!exit)
    {
        executeDispatchedMethods();

        listen();

        if (server_state != STATE_LOBBY && connected_clients == 0)
        {
            handleServerReset();
        }

        performStateBehaviour();
    }
}



void TronServer::listen()
{
    if (socket_selector.wait(sf::microseconds(100)))
    {
        if (socket_selector.isReady(tcp_listener))
        {
            // New connection request.
            acceptClient();
        }
        else
        {
            // Packet received.
            receivePacket();
        }
    }
}



void TronServer::handleServerReset()
{
    simulation_thread.eventResetSimulation();
    server_state = STATE_LOBBY;
}



void TronServer::performStateBehaviour()
{
    switch (server_state)
    {
        case STATE_LOBBY:
        {
            lobbyStateBehaviour();
            break;
        }

        case STATE_GAME:
        {
            gameStateBehaviour();
            break;
        }

        case STATE_END:
        {
            endStateBehaviour();
            break;
        }

        default: {}
    }
}



// Waits for players to ready up before starting the simulation.
void TronServer::lobbyStateBehaviour()
{
    if (connected_clients < MIN_PLAYERS ||
        clientExistsWithState(PlayerState::NOTREADY))
    {
        return;
    }

    simulation_thread.eventPrepareSimulation(getIDsInUse());
    server_state = STATE_GAME;

    sendUpdatedServerState();

    for (auto& client : clients)
    {
        if (!client)
        {
            continue;
        }

        client->setState(PlayerState::STARTING_GAME);
        sendUpdatedClientState(client);
        sendClientBulletin(client);
    }
}



void TronServer::gameStateBehaviour() const
{
    // Crickets ...
}



// Waits for all users to go back to back to lobby before changing server state.
void TronServer::endStateBehaviour()
{
    for (auto& client : clients)
    {
        if (!client)
        {
            continue;
        }

        if (client->getState() == PlayerState::VIEWING_RESULTS)
        {
            return;
        }
    }

    server_state = STATE_LOBBY;
    sendUpdatedServerBulletin();
}



/* Accepts a new client and informs them of server status.
 * Also informs all other connected clients of the new client.
 */
void TronServer::acceptClient()
{
    auto new_client = std::make_unique<Client>(generateUniqueClientID());

    if (tcp_listener.accept(*new_client->getSocket()) == sf::Socket::Done)
    {
        // Temporary server full code.
        if (new_client->getID() == MAX_PLAYERS)
        {
            new_client->getSocket()->disconnect();
            return;
        }

        socket_selector.add(*new_client->getSocket());
        
        sendClientIdentity(new_client);
        sendClientBulletin(new_client);
        sendClientList(new_client);
        sendClientJoined(new_client);

        clients[new_client->getID()] = std::move(new_client);
        ++connected_clients;
    }
}



// Receives a packet if one is available and passes it through for handling.
void TronServer::receivePacket()
{
    for (auto& sender : clients)
    {
        if (!sender)
        {
            continue;
        }

        if (socket_selector.isReady(*sender->getSocket()))
        {
            sf::Packet packet;
            if (sender->getSocket()->receive(packet) == sf::Socket::Disconnected)
            {
                return disconnectClient(sender);
            }

            handlePacket(packet, sender);
        }
    }
}



int TronServer::generateUniqueClientID() const
{
    int unique_id = 0;
    for (auto& client : clients)
    {
        if (!client)
        {
            return unique_id;
        }

        ++unique_id;
    }

    return MAX_PLAYERS;
}



std::vector<int> TronServer::getIDsInUse() const
{
    std::vector<int> ids;
    ids.reserve(MAX_PLAYERS);

    for (auto& client : clients)
    {
        if (!client)
        {
            continue;
        }

        ids.push_back(client->getID());
    }

    return ids;
}



bool TronServer::clientExistsWithState(const PlayerState _state)
{
    for (auto& client : clients)
    {
        if (!client)
        {
            continue;
        }

        if (client->getState() == _state)
        {
            return true;
        }
    }

    return false;
}



// Inform client of their ID on the server.
void TronServer::sendClientIdentity(ClientPtr& _client)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::IDENTITY);

    packet << _client->getID();

    sendPacketToClient(packet, _client);
}



// Inform client of the server's status.
void TronServer::sendClientBulletin(ClientPtr& _client)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::BULLETIN);

    std::string bulletin = server_state == STATE_LOBBY ?
        "Waiting for Players" : "Game in Progress";

    packet << bulletin;

    sendPacketToClient(packet, _client);
}



// Inform the client of all other connected clients, if any.
void TronServer::sendClientList(ClientPtr& _client)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::PLAYER_LIST);

    for (auto& client : clients)
    {
        if (!client)
        {
            continue;
        }

        packet << client->getID() << static_cast<sf::Uint8>(client->getState());
    }

    sendPacketToClient(packet, _client);
}



// Inform all clients of a new client connecting.
void TronServer::sendClientJoined(const ClientPtr& _client)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::PLAYER_JOINED);

    packet << _client->getID();

    sendPacketToAll(packet);
}



// Inform all clients of a client disconnecting.
void TronServer::sendClientLeft(ClientPtr& _client)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::PLAYER_LEFT);

    packet << _client->getID();

    sendPacketToAllButSender(packet, _client);
}



// Inform all clients of a change to a client's PlayerState.
void TronServer::sendUpdatedClientState(const ClientPtr& _client)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::PLAYER_STATE);

    packet << _client->getID() << static_cast<sf::Uint8>(_client->getState());

    sendPacketToAll(packet);
}



// Inform clients of a change to the server's state.
void TronServer::sendUpdatedServerState()
{
    sf::Packet packet;
    setPacketID(packet, PacketID::GAME_STATE);

    packet << static_cast<sf::Uint8>(server_state);

    sendPacketToAll(packet);
}



// Inform clients of a change to the server's simulation state.
void TronServer::sendUpdatedFlowControl(const FlowControl _control)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::FLOW_CONTROL);

    packet << static_cast<sf::Uint8>(_control);

    sendPacketToAll(packet);
}



// Inform all clients of the server's updated status.
void TronServer::sendUpdatedServerBulletin()
{
    for (auto& client : clients)
    {
        if (!client)
        {
            continue;
        }

        sendClientBulletin(client);
    }
}



/* Generic pass-through function which determines the appropriate 
 * packet handler function to call, based on those which have been registered.
 */
void TronServer::handlePacket(sf::Packet& _packet, ClientPtr& _sender)
{
    PacketID pid = getPacketID(_packet);
    packet_handlers.at(pid)(_packet, _sender);
}



void TronServer::handleDisconnectPacket(const sf::Packet& _packet, ClientPtr& _sender)
{
    disconnectClient(_sender);
}



// Send a PONG packet back to _sender to inform their latency.
void TronServer::handlePingPacket(const sf::Packet& _packet, ClientPtr& _sender)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::PONG);

    sendPacketToClient(packet, _sender);
}



void TronServer::handleMessagePacket(sf::Packet& _packet, ClientPtr& _sender)
{
    std::string msg;
    _packet >> msg;

    std::cout << msg << std::endl;

    sendPacketToAllButSender(_packet, _sender);
}



/* Resolves the change to the player state and sends the client's resolved state
 * to all other clients.
 */
void TronServer::handlePlayerStatePacket(sf::Packet& _packet, ClientPtr& _sender)
{
    switch (server_state)
    {
        case STATE_LOBBY:
        {
            // Player readied / unreadied.
            _sender->setState(_sender->getState() == PlayerState::NOTREADY ?
                PlayerState::READY : PlayerState::NOTREADY);
            break;
        }

        case STATE_GAME:
        {
            sf::Uint8 temp_state;
            _packet >> temp_state;

            PlayerState state = static_cast<PlayerState>(temp_state);

            if (state == PlayerState::NOTREADY)
            {
                // Player exited early or went back to lobby.
                _sender->setState(PlayerState::NOTREADY);
                simulation_thread.eventPlayerLeft(_sender->getID());
            }
            else if (state == PlayerState::PLAYING)
            {
                _sender->setState(state);

                // Only continue if all clients have loaded in.
                if (clientExistsWithState(PlayerState::STARTING_GAME))
                {
                    break;
                }

                sendUpdatedFlowControl(FlowControl::COUNTDOWN);
                simulation_thread.eventStartSimulation();
            }

            break;
        }

        case STATE_END:
        {
            // Player went back to lobby.
            _sender->setState(PlayerState::NOTREADY);

            break;
        }

        default: {}
    }

    sendUpdatedClientState(_sender);
}



// Pass on direction change request from client to the simulation thread.
void TronServer::handleDirectionPacket(sf::Packet& _packet, ClientPtr& _sender)
{
    if (server_state != STATE_GAME)
    {
        return;
    }

    sf::Uint8 dir;
    _packet >> dir;

    simulation_thread.eventDirectionChanged(_sender->getID(), 
        static_cast<MoveDirection>(dir));
}



// Pass on boost request from client to the simulation thread.
void TronServer::handleBoostPacket(const sf::Packet& _packet, ClientPtr& _sender)
{
    if (server_state != STATE_GAME)
    {
        return;
    }

    simulation_thread.eventBoost(_sender->getID());
}



void TronServer::disconnectClient(ClientPtr& _client)
{
    socket_selector.remove(*_client->getSocket());
    _client->getSocket()->disconnect();

    sendClientLeft(_client);

    if (_client->getState() == PlayerState::PLAYING)
    {
        simulation_thread.eventPlayerLeft(_client->getID());
    }

    _client->resetSocket();
    _client.reset();

    --connected_clients;
}



void TronServer::sendPacketToClient(sf::Packet& _packet, ClientPtr& _client)
{
    if (_client->getSocket()->send(_packet) == sf::Socket::Disconnected)
    {
        disconnectClient(_client);
    }
}



void TronServer::sendPacketToAll(sf::Packet& _packet)
{
    for (auto& client : clients)
    {
        if (!client)
        {
            continue;
        }

        sendPacketToClient(_packet, client);
    }
}



void TronServer::sendPacketToAllButSender(sf::Packet& _packet, const ClientPtr& _sender)
{
    for (auto& client : clients)
    {
        if (!client)
        {
            continue;
        }

        if (client->getSocket() == _sender->getSocket())
        {
            continue;
        }

        sendPacketToClient(_packet, client);
    }
}



void TronServer::sendPacketToAllPlaying(sf::Packet& _packet)
{
    for (auto& client : clients)
    {
        if (!client)
        {
            continue;
        }

        // Ignore clients who aren't in-game.
        if (client->getState() == PlayerState::PLAYING)
        {
            sendPacketToClient(_packet, client);
        }
    }
}



// Called by SimulationThread when the simulation needs to be synchronised.
void TronServer::onSyncSimulation(const SimulationState& _simulation)
{
    postEvent([this, _simulation]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::SYNC_SIMULATION);

        packet << _simulation;

        sendPacketToAll(packet);
    });
}



// Called by SimulationThread a bike needs to be synchronised.
void TronServer::onSyncBike(const BikeState& _bike)
{
    postEvent([this, _bike]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::SYNC_BIKE);

        packet << _bike;

        sendPacketToAll(packet);
    });
}



// Called by SimulationThread when all bikes need to be synchronised.
void TronServer::onSyncAllBikes(const std::array<BikeState, MAX_PLAYERS>& _bike_states)
{
    postEvent([this, _bike_states]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::SYNC_ALL_BIKES);

        for (auto& bike : _bike_states)
        {
            packet << bike;
        }

        sendPacketToAll(packet);
    });
}



// Called by SimulationThread when a bike has been removed from the simulation.
void TronServer::onBikeRemoved(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::BIKE_REMOVED);

        packet << static_cast<sf::Uint8>(_bike_id);

        sendPacketToAllPlaying(packet);
    });
}



// Called by SimulationThread when a bike has begun to boost.
void TronServer::onBikeBoost(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::BIKE_BOOST);

        packet << static_cast<sf::Uint8>(_bike_id);

        sendPacketToAllPlaying(packet);
    });
}



// Called by SimulationThread when a bike has been granted an additional boost charge.
void TronServer::onBoostChargeGranted(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::EXTRA_BOOST);

        packet << static_cast<sf::Uint8>(_bike_id);

        sendPacketToAllPlaying(packet);
    });
}



// Called by SimulationThread when the simulation has begun.
void TronServer::onSimulationStarted()
{
    postEvent([this]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::FLOW_CONTROL);

        packet << static_cast<sf::Uint8>(FlowControl::START);

        sendPacketToAllPlaying(packet);
    });
}



// Called by SimulationThread when the simulation is about to stop.
void TronServer::onSimulationStopping()
{
    postEvent([this]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::FLOW_CONTROL);

        packet << static_cast<sf::Uint8>(FlowControl::STOP);

        sendPacketToAllPlaying(packet);
    });
}



// Called by SimulationThread when the simulation has stopped.
void TronServer::onSimulationEnded()
{
    postEvent([this]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::GAME_STATE);

        server_state = STATE_END;
        packet << static_cast<sf::Uint8>(server_state);

        for (auto& client : clients)
        {
            if (!client)
            {
                continue;
            }

            // Send End state only to clients who were playing last round.
            if (client->getState() == PlayerState::PLAYING)
            {
                sendPacketToClient(packet, client);

                client->setState(PlayerState::VIEWING_RESULTS);
                sendUpdatedClientState(client);
            }
        }

        std::cout << "Simulation ended" << std::endl;
    });
}



// Called by SimulationThread when the simulation has been reset.
void TronServer::onSimulationReset()
{
    postEvent([this]()
    {
        server_state = STATE_LOBBY;
    });
}



// Called by SimulationThread when the simulation has found a victor.
void TronServer::onSimulationVictor(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::VICTOR);

        packet << static_cast<sf::Uint8>(_bike_id);

        std::cout << "Sim victor: " << _bike_id << std::endl;
        sendPacketToAll(packet);
    });
}
