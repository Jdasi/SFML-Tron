#pragma once

#include <map>
#include <functional>

#include <SFML/Network.hpp>

#include <Game/PacketID.h>
#include "ThreadDispatcher.h"
#include "SimpleTimer.h"

class NetworkException : public std::runtime_error
{
public:
    NetworkException() : std::runtime_error("Network exception") {}
};

class TronClient;

class NetworkManager : public ThreadDispatcher
{
public:
    NetworkManager(TronClient& client, const sf::IpAddress _ip_address, const unsigned int _tcp_port);
    ~NetworkManager();

    void connect();
    void disconnect();

private:
    void networkingThread();
    void stopNetworkingThread();

    void registerPacketHandlers();

    void handlePacket(sf::Packet& _packet);
    void handlePongPacket(sf::Packet& _packet);
    void handleMessagePacket(sf::Packet& _packet);

    void calculatePlayTime();

    void sendClientLatency();
    void sendPing();

    sf::IpAddress ip_address;
    unsigned int tcp_port;
    sf::TcpSocket socket;

    volatile bool has_connected;
    volatile bool running;
    volatile sf::Uint32 latency;
    double play_time;

    TronClient& client;

    SimpleTimer timer;

    std::thread network_thread;

    std::map<PacketID, std::function<void(sf::Packet&)>> packet_handlers;
};
