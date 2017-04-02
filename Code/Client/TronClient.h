#pragma once
#include <SFML/Graphics.hpp>

// TronClient includes headers for its members to avoid linking errors in main.
#include <Game/SimpleTimer.h>
#include <Game/Simulation.h>
#include <Game/ThreadDispatcher.h>
#include "INetworkClient.h"
#include "TronNetworkManager.h"
#include "InputHandler.h"
#include "ClientData.h"
#include "ClientStateHandler.h"
#include "GameManager.h"
#include "AssetManager.h"

class TronClient final : public INetworkClient, public ThreadDispatcher
{
public:
    TronClient();
    ~TronClient() = default;

    void run();
    void mainLoop();

    void onCommand(GameAction _action, ActionState _action_state) const;

private:
    void initKeyBindings();
    void initClientStates();

    void handleEvent(const sf::Event& _event);

    // Network events called by TronNetworkManager.
    void onConnected() override;
    void onDisconnected() override;
    void onUpdatePingTime(double _ping) override;
    void onIdentity(int _id) override;
    void onPlayerList(const std::vector<Player>& _players) override;
    void onPlayerJoined(int _id) override;
    void onPlayerStateChange(int _player_id, const PlayerState _state) override;
    void onGameStateChange(int _state) override;
    void onBikeSync(const BikeState& _bike_state) override;
    void onFullBikeSync(const std::array<BikeState, MAX_PLAYERS>& _bike_states) override;
    void onFullSync(const SimulationState& _simulation_state) override;
    void onBikeBoost(const unsigned int _bike_id) override;

    void tick();
    void draw();

    sf::RenderWindow window;
    sf::Font font;

    // Core systems.
    AssetManager asset_manager;
    TronNetworkManager network_manager;
    GameManager game_manager;
    InputHandler input_handler;
    ClientStateHandler state_handler;
    SimpleTimer timer;
    ClientData client_data;

};
