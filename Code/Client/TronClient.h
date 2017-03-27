#pragma once
#include <SFML/Graphics.hpp>

// TronClient includes headers for its members to avoid linking errors in main.
#include <Game/SimpleTimer.h>
#include <Game/Simulation.h>
#include "INetworkClient.h"
#include "ThreadDispatcher.h"
#include "TronNetworkManager.h"
#include "InputHandler.h"
#include "ClientData.h"
#include "ClientStateHandler.h"
#include "GameManager.h"

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

    // Network events called by TronNetworkManager.
    void onConnected() override;
    void onDisconnected() override;
    void onUpdatePingTime(double _ping) override;
    void onBikeDirectionChange(int _id, MoveDirection _dir) override;
    void onIdentity(int _id) override;
    void onPlayerList(std::vector<Player> _players) override;
    void onPlayerJoined(int _id) override;
    void onPlayerStateChange(int _player_id, PlayerState _state) override;
    void onGameStateChange(int _state) override;
    void onFullSync(Simulation& _simulation) override;

    void tick();
    void draw();

    sf::RenderWindow window;
    sf::Font font;

    // Core systems.
    TronNetworkManager network_manager;
    GameManager game_manager;
    InputHandler input_handler;
    ClientStateHandler state_handler;
    SimpleTimer timer;
    ClientData client_data;

};
