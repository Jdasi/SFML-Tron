#pragma once

#include <map>
#include <functional>

#include <SFML/Network.hpp>

#include <Game/PacketID.h>
#include "ThreadDispatcher.h"
#include "SimpleTimer.h"
#include "Scheduler.h"

class NetworkException : public std::runtime_error
{
public:
    NetworkException() : std::runtime_error("Network exception") {}
};

class TronClient;

class NetworkManager : public ThreadDispatcher
{
public:
    NetworkManager(const sf::IpAddress _ip_address, const unsigned int _tcp_port);
    ~NetworkManager();

    void connect();
    void disconnect();

protected:
    void registerPacketHandler(const PacketID _id, const std::function<void(sf::Packet&)> _handler);

    virtual void onConnected() = 0;
    virtual void onDisconnected() = 0;
    virtual void onUpdatePingTime(const sf::Uint32 _ping) = 0;

    void sendPacket(sf::Packet& _packet);

private:
    void networkingThread();
    void stopNetworkingThread();

    void registerPacketHandlers();
    void handlePacket(sf::Packet& _packet);
    void handlePongPacket(sf::Packet& _packet);


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

    SimpleTimer timer;
    Scheduler scheduler;

    std::thread network_thread;

    std::map<PacketID, std::function<void(sf::Packet&)>> packet_handlers;
};
