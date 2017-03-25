#pragma once
#include <SFML/Graphics.hpp>

// TronClient includes headers for its members to avoid linking errors in main.
#include <Game/SimpleTimer.h>
#include <Game/Simulation.h>
#include "NetworkClient.h"
#include "ThreadDispatcher.h"
#include "TronNetworkManager.h"
#include "InputHandler.h"
#include "ClientData.h"
#include "ClientStateHandler.h"
#include "PlayerManager.h"

class TronClient : public INetworkClient, public ThreadDispatcher
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

    void onConnected() override;
    void onDisconnected() override;
    void onUpdatePingTime(double _ping) override;
    void onBikeDirectionChange(int _id, MoveDirection _dir) override;
    void onIdentity(int _id) override;
    void onPlayerJoined(int _id) override;

    void tick();
    void draw();

    sf::RenderWindow window;
    sf::Font font;

    TronNetworkManager network_manager;
    InputHandler input_handler;
    ClientData client_data;
    ClientStateHandler state_handler;
    SimpleTimer timer;
    Simulation simulation;
    PlayerManager player_manager;

};
