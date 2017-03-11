#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

// TronClient includes headers for its members to avoid linking errors in main.
#include "SimpleTimer.h"
#include "GameAction.h"
#include "ObjectRenderer.h"
#include "InputHandler.h"
#include "ClientStateHandler.h"
#include "ClientStates.h"
#include "ClientData.h"
#include "ThreadDispatcher.h"
#include "TronNetworkManager.h"
#include "NetworkClient.h"

class TronClient : public INetworkClient, public ThreadDispatcher
{
public:
    TronClient(sf::IpAddress _ip_address, unsigned int _tcp_port);
    ~TronClient() = default;

    void run();
    void onCommand(GameAction _action, ActionState _action_state) const;

private:
    void onConnected() override;
    void onDisconnected() override;
    void onUpdatePingTime(const sf::Uint32 _ping) override;

    void handleEvent(const sf::Event& _event);

    sf::RenderWindow window;
    sf::Font font;

    TronNetworkManager network_manager;
    ObjectRenderer object_renderer;
    InputHandler input_handler;
    ClientData client_data;
    ClientStateHandler state_handler;
    SimpleTimer timer;
};
