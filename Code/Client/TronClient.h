#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

// TronClient includes headers for its members to avoid linking errors in main.
#include <Game/SimpleTimer.h>
#include "GameAction.h"
#include "InputHandler.h"
#include "ClientStateHandler.h"
#include "ClientStates.h"
#include "ClientData.h"

class TronClient
{
public:
    TronClient();
    ~TronClient() = default;

    void run();
    void onCommand(GameAction _action, ActionState _action_state) const;

private:
    void initKeyBindings();
    void initClientStates();

    void handleEvent(const sf::Event& _event);

    void tick();
    void draw();

    sf::RenderWindow window;
    sf::Font font;

    InputHandler input_handler;
    ClientData client_data;
    ClientStateHandler state_handler;
    SimpleTimer timer;
};
