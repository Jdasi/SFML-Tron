#pragma once
#include <memory>
#include <queue>
#include <mutex>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <Game/Constants.h>
#include "SimpleTimer.h"
#include "NetworkManager.h"
#include "GameAction.h"
#include "ObjectRenderer.h"
#include "InputHandler.h"
#include "ClientStateHandler.h"
#include "ClientStates.h"
#include "ClientData.h"
#include "ThreadDispatcher.h"

class TronClient : public ThreadDispatcher
{
public:
    TronClient(sf::IpAddress _ip_address, unsigned int _tcp_port);
    ~TronClient() = default;

    void run();
    void onCommand(GameAction _action, ActionState _action_state) const;

    void updatePingTime(const double ping);

private:
    void handleEvent(const sf::Event& _event) const;

    NetworkManager network_manager;

    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<ObjectRenderer> object_renderer;
    std::unique_ptr<InputHandler> input_handler;
    std::unique_ptr<ClientData> client_data;
    std::unique_ptr<ClientStateHandler> state_handler;

    SimpleTimer timer;
};
