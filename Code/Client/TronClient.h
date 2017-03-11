#pragma once
#include <memory>
#include <queue>
#include <mutex>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <Game/PacketID.h>
#include <Game/Constants.h>
#include "SimpleTimer.h"
#include "GameAction.h"
#include "ObjectRenderer.h"
#include "InputHandler.h"
#include "ClientStateHandler.h"
#include "ClientStates.h"
#include "ClientData.h"

class TronClient
{
public:
    TronClient(sf::IpAddress _ip_address, unsigned int _tcp_port);
    ~TronClient() = default;

    void run();
    void onCommand(GameAction _action, ActionState _action_state) const;

private:
    void handleEvent(const sf::Event& _event) const;
    bool connect();
    void receive();
    void ping();
    void shutdown();
    void handlePacket(sf::Packet& _packet);

    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<ObjectRenderer> object_renderer;
    std::unique_ptr<InputHandler> input_handler;
    std::unique_ptr<ClientData> client_data;
    std::unique_ptr<ClientStateHandler> state_handler;

    SimpleTimer timer;

    sf::IpAddress ip_address;
    unsigned int tcp_port;
    sf::TcpSocket socket;

    std::queue<sf::Packet> pong_queue;
    std::mutex pong_queue_mutex;

};
