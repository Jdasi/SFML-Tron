#pragma once
#include <map>
#include <functional>
#include <thread>

#include <SFML/Network.hpp>

#include <Game/PacketID.h>
#include <Game/Scheduler.h>
#include <Game/SimpleTimer.h>
#include "ThreadDispatcher.h"

class NetworkException : public std::runtime_error
{
public:
    NetworkException() : std::runtime_error("Network exception") {}
};

class NetworkManager : public ThreadDispatcher
{
public:
    NetworkManager(const sf::IpAddress _ip_address, const unsigned int _tcp_port);
    ~NetworkManager();

    void connect();
    void disconnect();

protected:
    // Required network events.
    virtual void onConnected() = 0;
    virtual void onDisconnected() = 0;
    virtual void onUpdatePingTime(double _ping) = 0;

    void sendPacket(sf::Packet& _packet);

    std::map<PacketID, std::function<void(sf::Packet&)>> packet_handlers;

private:
    void networkingThread();
    void stopNetworkingThread();

    void registerPacketHandlers();

    // Generic network packet handlers.
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
    volatile double latency;
    double play_time;

    SimpleTimer timer;
    Scheduler scheduler;

    std::thread network_thread;

};
