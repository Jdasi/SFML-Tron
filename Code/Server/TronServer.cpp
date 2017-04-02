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

bool TronServer::run()
{
    if (!bindServerPort())
    {
        return false;
    }

    std::cout << "Server launched on port: " << SERVER_TCP_PORT << std::endl
              << "> Awaiting clients . . ." << std::endl;

    socket_selector.add(tcp_listener);

    mainLoop();

    return true;
}

void TronServer::registerPacketHandlers()
{
    registerPacketHandler(PacketID::DISCONNECT,   handleDisconnectPacket);
    registerPacketHandler(PacketID::PING,         handlePingPacket);
    registerPacketHandler(PacketID::LATENCY,      handleLatencyPacket);
    registerPacketHandler(PacketID::MESSAGE,      handleMessagePacket);
    registerPacketHandler(PacketID::PLAYER_STATE, handlePlayerStatePacket);
    registerPacketHandler(PacketID::DIRECTION,    handleDirectionPacket);
    registerPacketHandler(PacketID::BOOST,        handleBoostPacket);
}

bool TronServer::bindServerPort()
{
    if (tcp_listener.listen(SERVER_TCP_PORT) != sf::Socket::Done)
    {
        std::cout << "Could not bind server port: " << SERVER_TCP_PORT << std::endl;

        return false;
    }

    return true;
}

void TronServer::mainLoop()
{
    while (!exit)
    {
        executeDispatchedMethods();
        scheduler.update();

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
    if (socket_selector.wait(sf::milliseconds(1)))
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
        } break;

        case STATE_GAME:
        {
            gameStateBehaviour();
        } break;

        case STATE_END:
        {
            endStateBehaviour();
        } break;

        default: {}
    }
}

void TronServer::lobbyStateBehaviour()
{
    if (connected_clients == 0)
    {
        return;
    }

    for (auto& client : clients)
    {
        if (!client)
        {
            continue;
        }

        if (client->getState() == PlayerState::NOTREADY)
        {
            return;
        }
    }

    allClientsReady();
}

void TronServer::gameStateBehaviour()
{
    // Crickets ...
}

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
}

void TronServer::allClientsReady()
{
    std::vector<int> bike_ids;
    bike_ids.reserve(MAX_PLAYERS);

    for (auto& client : clients)
    {
        if (!client)
        {
            continue;
        }

        bike_ids.push_back(client->getID());
    }

    simulation_thread.eventStartSimulation(bike_ids);
    server_state = STATE_GAME;
}

void TronServer::acceptClient()
{
    auto new_client = std::make_unique<Client>(generateUniqueID());

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
        sendClientList(new_client);
        sendClientJoined(new_client);

        clients[new_client->getID()] = std::move(new_client);
        ++connected_clients;
    }
}

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

int TronServer::generateUniqueID() const
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

void TronServer::sendClientIdentity(ClientPtr& _client)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::IDENTITY);

    packet << _client->getID();

    sendPacketToClient(packet, _client);
}

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

void TronServer::sendClientJoined(const ClientPtr& _client)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::PLAYER_JOINED);

    packet << _client->getID();

    sendPacketToAll(packet);
}

void TronServer::sendClientLeft(ClientPtr& _client)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::PLAYER_LEFT);

    packet << _client->getID();

    sendPacketToAllButSender(packet, _client);
}

void TronServer::sendUpdatedClientState(const ClientPtr& _client)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::PLAYER_STATE);

    packet << _client->getID() << static_cast<sf::Uint8>(_client->getState());

    sendPacketToAll(packet);
}

void TronServer::handlePacket(sf::Packet& _packet, ClientPtr& _sender)
{
    PacketID pid = getPacketID(_packet);
    packet_handlers.at(pid)(_packet, _sender);
}

void TronServer::handleDisconnectPacket(sf::Packet& _packet, ClientPtr& _sender)
{
    disconnectClient(_sender);
}

// Send a PONG packet back to _sender to inform their latency.
void TronServer::handlePingPacket(sf::Packet& _packet, ClientPtr& _sender)
{
    double time_stamp = 0;
    _packet >> time_stamp;

    std::cout << "Ping Packet Received: " << time_stamp << std::endl;

    sf::Packet packet;
    setPacketID(packet, PacketID::PONG);

    std::cout << "Sending Pong Packet" << std::endl;

    packet << time_stamp;
    sendPacketToClient(packet, _sender);
}

void TronServer::handleLatencyPacket(sf::Packet& _packet, ClientPtr& _sender) const
{
    double latency;
    _packet >> latency;

    _sender->setLatency(latency);
    std::cout << "Client " << static_cast<int>(_sender->getID()) << ": " 
              << latency << "ms" << std::endl;
}

void TronServer::handleMessagePacket(sf::Packet& _packet, ClientPtr& _sender)
{
    std::string msg;
    _packet >> msg;

    std::cout << msg << std::endl;

    sendPacketToAllButSender(_packet, _sender);
}

void TronServer::handlePlayerStatePacket(const sf::Packet& _packet, ClientPtr& _sender)
{
    switch (server_state)
    {
        case STATE_LOBBY:
        {
            // Player readied / unreadied.
            _sender->setState(_sender->getState() == PlayerState::NOTREADY ?
                PlayerState::READY : PlayerState::NOTREADY);
        } break;

        case STATE_GAME:
        {
            // Player exited early, or went back to lobby.
            _sender->setState(PlayerState::NOTREADY);

            simulation_thread.eventPlayerLeft(_sender->getID());
        }

        case STATE_END:
        {
            // Player went back to lobby.
            _sender->setState(PlayerState::NOTREADY);

            sf::Packet packet;
            setPacketID(packet, PacketID::GAME_STATE);

            packet << static_cast<sf::Uint8>(STATE_LOBBY);

            sendPacketToClient(packet, _sender);
        } break;

        default: {}
    }

    sendUpdatedClientState(_sender);
}

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

void TronServer::handleBoostPacket(sf::Packet& _packet, ClientPtr& _sender)
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

void TronServer::onBikeBoost(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::BOOST);

        packet << static_cast<sf::Uint8>(_bike_id);

        sendPacketToAll(packet);
    });
}

void TronServer::onSimulationStarted()
{
    postEvent([this]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::GAME_STATE);

        server_state = STATE_GAME;
        packet << static_cast<sf::Uint8>(server_state);

        sendPacketToAll(packet);

        for (auto& client : clients)
        {
            if (!client)
            {
                continue;
            }

            client->setState(PlayerState::PLAYING);
            sendUpdatedClientState(client);
        }

        std::cout << "Simulation started" << std::endl;
    });
}

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