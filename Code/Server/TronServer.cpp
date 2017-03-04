#include <iostream>

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
                // Message received.
                receiveMessage();
            }
        }

        garbageCollectClients();
    }
}

void TronServer::acceptClient()
{
    auto new_user = std::make_unique<User>();
    std::string connection_message("User connected");

    if (tcp_listener.accept(*new_user->getSocket()) == sf::Socket::Done)
    {
        std::cout << connection_message << std::endl;

        socket_selector.add(*new_user->getSocket());
        users.push_back(std::move(new_user));

        sf::Packet packet;
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

void TronServer::receiveMessage()
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

            std::string message;
            packet >> message;

            if (message == "disconnectme")
            {
                return handleDisconnect(sender);
            }

            std::cout << message << std::endl;

            for (auto& user : users)
            {
                if (user->getSocket() == sender->getSocket())
                    continue;

                user->getSocket()->send(packet);
            }
        }
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
        [](const std::unique_ptr<User>& user) { return !user->getSocket(); }),
        users.end());

    clients_dirty = false;
}

void TronServer::handleDisconnect(std::unique_ptr<User>& user)
{
    socket_selector.remove(*user->getSocket());
    user->getSocket()->disconnect();

    user->resetSocket();
    clients_dirty = true;

    std::string disconnection_message("User disconnected");
    std::cout << disconnection_message << std::endl;

    sf::Packet packet;
    packet << disconnection_message;
    for (auto& u : users)
    {
        if (!u->getSocket())
            continue;

        u->getSocket()->send(packet);
    }

    --connected_clients;
}
