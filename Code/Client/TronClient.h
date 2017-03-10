#pragma once
#include <memory>
#include <chrono>
#include <queue>
#include <mutex>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <Game/TronGame.h>
#include "InputHandler.h"
#include "SimpleTimer.h"
#include "ObjectRenderer.h"
#include "ClientStateHandler.h"
#include "ClientData.h"

class TronClient
{
public:
    TronClient();
    ~TronClient() = default;

    void run();
    void onCommand(GameAction _action, ActionState _action_state) const;

private:
    void handleEvent(const sf::Event& _event) const;

    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<ObjectRenderer> object_renderer;
    std::unique_ptr<InputHandler> input_handler;
    std::unique_ptr<ClientData> client_data;
    std::unique_ptr<ClientStateHandler> state_handler;

    SimpleTimer timer;

};
