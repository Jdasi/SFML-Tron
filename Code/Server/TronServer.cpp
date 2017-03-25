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
        float dt = simple_timer.getTimeDifference();
        simple_timer.reset();

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

        if (connected_clients > 0)
        {
            simulation.tick(dt);
        }

        garbageCollectClients();
    }
}

void TronServer::acceptClient()
{
    auto new_user = std::make_unique<User>(connected_clients);

    if (tcp_listener.accept(*new_user->getSocket()) == sf::Socket::Done)
    {
        // Temporary server full code.
        if (connected_clients >= 4)
        {
            new_user->getSocket()->disconnect();
            return;
        }

        socket_selector.add(*new_user->getSocket());
        
        sf::Packet packet;
        setPacketID(packet, PacketID::IDENTITY);
        packet << new_user->getID();
        new_user->getSocket()->send(packet);

        setPacketID(packet, PacketID::PLAYERJOINED);
        packet << new_user->getID();

        for (auto& user : users)
        {
            user->getSocket()->send(packet);
        }

        users.push_back(std::move(new_user));
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
            sf::Uint8 id;
            sf::Uint8 dir;

            _packet >> id >> dir;
            // Do something with server's simulation ...

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
