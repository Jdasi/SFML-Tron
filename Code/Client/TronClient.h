#pragma once
#include <SFML/Graphics.hpp>

// TronClient includes headers for its members to avoid linking errors in main.
#include <Game/SimpleTimer.h>
#include <Game/Simulation.h>
#include <Game/ThreadDispatcher.h>
#include <Game/ServerSettings.h>
#include "INetworkClient.h"
#include "NetworkManager.h"
#include "InputHandler.h"
#include "ClientData.h"
#include "ClientStateHandler.h"
#include "GameManager.h"
#include "AssetManager.h"
#include "GameAudio.h"

/* Main class which owns the majority of client-side functionality.
 * TronClient and most of its members reside in one thread, with the exception of
 * NetworkManager, which resides in its own thread.
 *
 * The two threads post events to each other via the ThreadDispatcher class,
 * allowing for messages to cross the thread boundary with minimal interruption
 * to the runtime behaviour of either thread.
 */
class TronClient final : public INetworkClient, public ThreadDispatcher
{
public:
    explicit TronClient(const ServerSettings& _server_settings);
    ~TronClient() = default;

    void run();
    void mainLoop();

    void onCommand(GameAction _action, ActionState _action_state) const;
    bool isExiting() const override;

private:
    void preloadSoundBuffers();
    void initKeyboardBindings();
    void initControllerBindings();
    void initClientStates();
    void initTextObjects();

    // SFML events.
    void handleEvent(const sf::Event& _event);

    // ThreadDispatcher events called by NetworkManager.
    void onConnected() override;
    void onDisconnected() override;
    void onUpdatePingTime(const double _ping) override;
    void onIdentity(const unsigned int _player_id) override;
    void onPlayerList(const std::vector<Player>& _players) override;
    void onPlayerJoined(const unsigned int _player_id) override;
    void onPlayerLeft(const unsigned int _player_id) override;
    void onPlayerStateChange(const unsigned int _player_id, const PlayerState _state) override;
    void onGameStateChange(const int _state) override;
    void onFlowControl(const FlowControl _control) override;
    void onVictor(const unsigned int _player_id) override;
    void onBikeSync(const BikeState& _bike_state) override;
    void onFullBikeSync(const std::array<BikeState, MAX_PLAYERS>& _bike_states) override;
    void onFullSync(const SimulationState& _simulation_state) override;
    void onBikeRemoved(const unsigned int _bike_id) override;
    void onBikeBoost(const unsigned int _bike_id) override;
    void onBoostChargeGranted(const unsigned int _bike_id) override;
    void onBulletinUpdate(const std::string& _bulletin) override;

    void tick();
    void draw();

    void updateServerReadout();

    sf::RenderWindow window;

    // Core systems & data.
    AssetManager asset_manager;
    NetworkManager network_manager;
    GameManager game_manager;
    InputHandler input_handler;
    ClientStateHandler state_handler;
    SimpleTimer timer;
    GameAudio game_audio;
    ClientData client_data;

    bool in_focus;
    sf::Text server_readout;

    // For the purposes of printing server info to the screen..
    std::string ip_address;
    unsigned int tcp_port;

};
