#include <iostream>

#include <Game/Constants.h>
#include <Game/PacketID.h>
#include <Game/GameStateIDs.h>
#include "TronServer.h"

TronServer::TronServer()
    : tcp_port(0)
    , connected_clients(0)
    , exit(false)
    , server_name()
    , server_state(STATE_LOBBY)
    , full_sync_needed(true)
{
    clients.reserve(MAX_PLAYERS);

    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        clients.push_back(nullptr);
    }
}

bool TronServer::run(unsigned int port)
{
    tcp_port = port;

    if (!bindServerPort())
    {
        return false;
    }

    std::cout << "Server launched on port: " << tcp_port << std::endl
              << "> Awaiting clients . . ." << std::endl;

    socket_selector.add(tcp_listener);

    listen();

    return true;
}

void TronServer::stop()
{
    exit = true;
}

bool TronServer::bindServerPort()
{
    if (tcp_listener.listen(tcp_port) != sf::Socket::Done)
    {
        std::cout << "Could not bind server port: " << tcp_port << std::endl;

        return false;
    }

    return true;
}

void TronServer::listen()
{
    while (!exit)
    {
        double dt = simple_timer.getTimeDifference();
        simple_timer.reset();
        scheduler.update();

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

        if (server_state == STATE_GAME)
        {
            simulation.tick(dt);

            if (full_sync_needed)
            {
                full_sync_needed = false;

                scheduler.invoke([this]() { fullSimulationSync(); }, 0.1);
            }
        }
    }
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

void TronServer::sendClientIdentity(std::unique_ptr<Client>& _client) const
{
    sf::Packet packet;
    setPacketID(packet, PacketID::IDENTITY);

    packet << _client->getID();

    _client->getSocket()->send(packet);
}

void TronServer::sendClientList(std::unique_ptr<Client>& _client)
{
    if (clients.size() == 0)
    {
        return;
    }

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

    _client->getSocket()->send(packet);
}

void TronServer::sendClientJoined(std::unique_ptr<Client>& _client)
{
    sf::Packet packet;
    setPacketID(packet, PacketID::PLAYER_JOINED);

    packet << _client->getID();

    sendPacketToAll(packet);
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
                return handleDisconnect(sender);
            }

            handlePacket(packet, sender);
        }
    }
}

void TronServer::handlePacket(sf::Packet& _packet, std::unique_ptr<Client>& _sender)
{
    PacketID pid = getPacketID(_packet);

    switch (pid)
    {
        case DISCONNECT:
        {
            handleDisconnect(_sender);
        } break;

        // Send a PONG packet back to _sender to inform their latency.
        case PING:
        {
            double time_stamp = 0;
            _packet >> time_stamp;

            std::cout << "Ping Packet Received: " << time_stamp << std::endl;

            sf::Packet packet;
            setPacketID(packet, PacketID::PONG);

            std::cout << "Sending Pong Packet" << std::endl;

            packet << time_stamp;
            _sender->getSocket()->send(packet);
        } break;

        case LATENCY:
        {
            double latency;
            _packet >> latency;

            _sender->setLatency(latency);
            std::cout << "Client " << static_cast<int>(_sender->getID()) << ": " << latency << "ms" << std::endl;
        } break;

        case MESSAGE:
        {
            std::string msg;
            _packet >> msg;
            std::cout << msg << std::endl;

            sendPacketToAllButSender(_packet, _sender);
        } break;

        case PLAYER_STATE:
        {
            sf::Uint8 id;
            sf::Uint8 state;

            _packet >> id >> state;
            clients[id]->setState(static_cast<PlayerState>(state));

            sendPacketToAll(_packet);

            if (server_state == STATE_LOBBY)
            {
                int num_ready = 0;
                for (auto& client : clients)
                {
                    if (!client)
                    {
                        continue;
                    }

                    num_ready += client->getState() == PlayerState::READY ? 1 : -1;
                }

                if (num_ready == connected_clients)
                {
                    sf::Packet packet;
                    setPacketID(packet, PacketID::GAME_STATE);

                    server_state = STATE_GAME;
                    packet << static_cast<sf::Uint8>(STATE_GAME);

                    sendPacketToAll(packet);

                    for (int i = 0; i < connected_clients; ++i)
                    {
                        simulation.addBike();
                    }

                    fullSimulationSync();
                }
            }
        } break;

        case DIRECTION:
        {
            sf::Uint8 id;
            sf::Uint8 dir;

            _packet >> id >> dir;
            simulation.changeBikeDirection(id, static_cast<MoveDirection>(dir));

            sendPacketToAllButSender(_packet, _sender);
        } break;

        default: {}
    }
}

void TronServer::handleDisconnect(std::unique_ptr<Client>& _client)
{
    socket_selector.remove(*_client->getSocket());
    _client->getSocket()->disconnect();

    std::string disconnection_message;
    disconnection_message.append("[Client " + std::to_string(_client->getID()) + " disconnected]");
    std::cout << disconnection_message << std::endl;

    _client->resetSocket();
    _client.reset();

    sf::Packet packet;
    setPacketID(packet, PacketID::MESSAGE);
    packet << disconnection_message;

    sendPacketToAll(packet);
    --connected_clients;
}

void TronServer::sendPacketToAll(sf::Packet& _packet)
{
    for (auto& client : clients)
    {
        if (!client)
        {
            continue;
        }

        client->getSocket()->send(_packet);
    }
}

void TronServer::sendPacketToAllButSender(sf::Packet& _packet, std::unique_ptr<Client>& _sender)
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

        client->getSocket()->send(_packet);
    }
}

void TronServer::fullSimulationSync()
{
    if (server_state != STATE_GAME)
    {
        return;
    }

    sf::Packet packet;
    setPacketID(packet, PacketID::SYNC_SIMULATION);

    packet << simulation;
    sendPacketToAll(packet);

    full_sync_needed = true;

    std::cout << "Full Sync sent" << std::endl;
}
