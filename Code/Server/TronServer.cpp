#include <iostream>

#include <Game/PacketID.h>
#include "TronServer.h"

TronServer::TronServer()
    : tcp_port(0)
    , exit(false)
    , connected_clients(0)
    , clients_dirty(false)
    , server_name()
{
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
        if (socket_selector.wait())
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

        garbageCollectClients();
    }
}

void TronServer::acceptClient()
{
    auto new_user = std::make_unique<User>(connected_clients);
    std::string connection_message;
    connection_message.append("[User " + std::to_string(connected_clients) + " connected]");

    if (tcp_listener.accept(*new_user->getSocket()) == sf::Socket::Done)
    {
        std::cout << connection_message << std::endl;

        socket_selector.add(*new_user->getSocket());
        users.push_back(std::move(new_user));

        sf::Packet packet;
        setPacketID(packet, PacketID::MESSAGE);
        packet << connection_message;

        for (auto& user : users)
        {
            if (user == users[connected_clients])
                continue;

            user->getSocket()->send(packet);
        }

        ++connected_clients;
    }
}

void TronServer::receivePacket()
{
    for (auto& sender : users)
    {
        if (socket_selector.isReady(*sender->getSocket()))
        {
            sf::Packet packet;
            if (sender->getSocket()->receive(packet) == sf::Socket::Disconnected)
            {
                return handleDisconnect(sender);
            }

            std::cout << "Received Packet" << std::endl;

            handlePacket(packet, sender);
        }
    }
}

void TronServer::handlePacket(sf::Packet& _packet, std::unique_ptr<User>& _sender)
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

            std::cout << "Ping Packet Received: " <<time_stamp << std::endl;

            sf::Packet packet;
            setPacketID(packet, PacketID::PONG);

            std::cout << "Sending Pong Packet" << std::endl;

            packet << time_stamp;
            _sender->getSocket()->send(packet);
        } break;

        case LATENCY:
        {
            sf::Uint32 latency;
            _packet >> latency;
            _sender->setLatency(latency);
            std::cout << "User " << static_cast<int>(_sender->getID()) << ": " << latency << "ms" << std::endl;
        } break;

        case MESSAGE:
        {
            std::string msg;
            _packet >> msg;
            std::cout << msg << std::endl;

            for (auto& user : users)
            {
                // Don't send the sender's message back to themself.
                if (user->getSocket() == _sender->getSocket())
                {
                    continue;
                }

                user->getSocket()->send(_packet);
            }
        } break;

        case DIRECTION:
        {
            for (auto& user : users)
            {
                // Don't send the sender's message back to themself.
                if (user->getSocket() == _sender->getSocket())
                {
                    continue;
                }

                user->getSocket()->send(_packet);
            }
        } break;

        default: {}
    }
}

void TronServer::garbageCollectClients()
{
    if (!clients_dirty)
    {
        return;
    }

    users.erase(std::remove_if(
        users.begin(),
        users.end(),
        [](const std::unique_ptr<User>& _user) { return !_user->getSocket(); }),
        users.end());

    clients_dirty = false;
}

void TronServer::handleDisconnect(std::unique_ptr<User>& _user)
{
    socket_selector.remove(*_user->getSocket());
    _user->getSocket()->disconnect();

    _user->resetSocket();
    clients_dirty = true;

    std::string disconnection_message;
    disconnection_message.append("[User " + std::to_string(_user->getID()) + " disconnected]"); 
    std::cout << disconnection_message << std::endl;

    sf::Packet packet;
    setPacketID(packet, PacketID::MESSAGE);
    packet << disconnection_message;

    for (auto& user : users)
    {
        if (!user->getSocket())
        {
            continue;
        }

        user->getSocket()->send(packet);
    }

    --connected_clients;
}
