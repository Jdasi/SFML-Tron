#include <iostream>

#include "NetworkManager.h"

using namespace std::placeholders;

#define registerPacketHandler(id, func) \
    packet_handlers.emplace(id, std::bind(&NetworkManager::func, this, _1))

NetworkManager::NetworkManager(const sf::IpAddress _ip_address, const unsigned int _tcp_port)
    : packet_handlers()
    , ip_address(_ip_address)
    , tcp_port(_tcp_port)
    , socket()
    , has_connected(false)
    , running(true)
    , latency(0)
    , play_time(0)
    , timer()
    , scheduler()
{
    registerPacketHandlers();

    network_thread = std::thread([this]()
    {
        std::cout << "Networking thread started." << std::endl;
        networkingThread();
        std::cout << "Networking thread stopping." << std::endl;
    });
}

NetworkManager::~NetworkManager()
{
    stopNetworkingThread();
}

void NetworkManager::connect()
{
    postEvent([this]()
    {
        socket.setBlocking(true);

        auto status = socket.connect(ip_address, tcp_port);
        if (status != sf::Socket::Done)
        {
            onDisconnected();
            return;
        }

        socket.setBlocking(false);
        has_connected = true;

        onConnected();

        sendPing();

        std::cout << "Connected to server: " << ip_address << std::endl;
    });
}

void NetworkManager::disconnect()
{
    postEvent([this]()
    {
        sf::Packet packet;
        setPacketID(packet, PacketID::DISCONNECT);

        socket.send(packet);
    });
}

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
            } break;

            case sf::Socket::Disconnected:
            case sf::Socket::Error:
            {
                running = false;
                onDisconnected();
                return;
            }

            case sf::Socket::NotReady:
            case sf::Socket::Partial:
            default:
            {
            } break;
        }
    }

    std::cout << "Networking thread ended" << std::endl;
}

void NetworkManager::stopNetworkingThread()
{
    running = false;
    network_thread.join();
    std::cout << "Networking thread stopped." << std::endl;
}

void NetworkManager::registerPacketHandlers()
{
    registerPacketHandler(PONG, handlePongPacket);
}

void NetworkManager::handlePacket(sf::Packet& _packet)
{
    PacketID pid = getPacketID(_packet);
    packet_handlers.at(pid)(_packet);
}

void NetworkManager::handlePongPacket(sf::Packet& _packet)
{
    std::cout << "Received pong." << std::endl;

    double prev_play_time = 0;
    _packet >> prev_play_time;

    latency = (play_time - prev_play_time) * 1000;

    onUpdatePingTime(latency);

    sendClientLatency();

    scheduler.invoke([this](){ sendPing(); }, 1.0);
}

void NetworkManager::sendPacket(sf::Packet& _packet)
{
    while (socket.send(_packet) == sf::Socket::Partial){}
}

void NetworkManager::calculatePlayTime()
{
    play_time += timer.getTimeDifference();
    timer.reset();
}

void NetworkManager::sendClientLatency()
{
    sf::Packet packet;
    setPacketID(packet, PacketID::LATENCY);

    packet << latency;
    
    sendPacket(packet);
}

void NetworkManager::sendPing()
{
    calculatePlayTime();

    sf::Packet packet;
    setPacketID(packet, PacketID::PING);

    packet << play_time;
    sendPacket(packet);
}
