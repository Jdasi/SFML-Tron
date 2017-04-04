#include <SFML/Graphics.hpp>

#include <Game/Constants.h>
#include <Game/GameStateIDs.h>
#include <Game/JHelper.h>
#include "TronClient.h"
#include "ClientStateLobby.h"
#include "ClientStateGame.h"
#include "ClientStateEnd.h"

TronClient::TronClient()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME)
    , network_manager(*this, SERVER_IP, SERVER_TCP_PORT)
    , game_manager(&client_data)
    , input_handler(*this)
    , state_handler()
    , game_audio(&asset_manager, &in_focus)
    , client_data(&asset_manager, &network_manager, &game_manager, &input_handler, &game_audio)
    , in_focus(true)
{
    preloadSoundBuffers();
    initTextObjects();
}



void TronClient::run()
{
    initKeyboardBindings();
    initControllerBindings();
    initClientStates();

    network_manager.connect();

    mainLoop();

    window.close();
}



void TronClient::mainLoop()
{
    while (!client_data.exit)
    {
        // Crude delta-time system.
        client_data.delta_time = timer.getTimeDifference();
        timer.reset();
        client_data.play_time += client_data.delta_time;

        executeDispatchedMethods();

        tick();
        draw();

        // Handle events.
        sf::Event event;
        while (window.pollEvent(event))
        {
            handleEvent(event);
        }
    }
}



// Forwards any input-based commands to the current state for processing.
void TronClient::onCommand(const GameAction _action, const ActionState _action_state) const
{
    state_handler.onCommand(_action, _action_state);
}



bool TronClient::isExiting() const
{
    return client_data.exit;
}



void TronClient::preloadSoundBuffers()
{
    asset_manager.loadSoundBuffer(COUNTDOWN_TICK_CUE);
    asset_manager.loadSoundBuffer(COUNTDOWN_FIN_CUE);
    asset_manager.loadSoundBuffer(BOOST_CUE);
    asset_manager.loadSoundBuffer(DEATH_CUE);
    asset_manager.loadSoundBuffer(ROUND_OVER_CUE);
    asset_manager.loadSoundBuffer(WINNER_CUE);
    asset_manager.loadSoundBuffer(LOSER_CUE);
}

void TronClient::initTextObjects()
{
    auto font = asset_manager.loadFont(DEFAULT_FONT);

    server_readout = std::make_unique<sf::Text>("", *font);
    server_readout->setCharacterSize(20);
    server_readout->setStyle(sf::Text::Bold);
    server_readout->setFillColor(sf::Color::White);
    server_readout->setPosition({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 1.05f });
    server_readout->setOutlineColor(sf::Color::Black);
    server_readout->setOutlineThickness(1.5f);
}


void TronClient::initKeyboardBindings()
{
    input_handler.registerKeyboardKey(sf::Keyboard::Key::Escape, GameAction::QUIT);
    input_handler.registerKeyboardKey(sf::Keyboard::Key::Return, GameAction::ACCEPT);

    input_handler.registerKeyboardKey(sf::Keyboard::Key::W, GameAction::MOVE_UP);
    input_handler.registerKeyboardKey(sf::Keyboard::Key::S, GameAction::MOVE_DOWN);
    input_handler.registerKeyboardKey(sf::Keyboard::Key::A, GameAction::MOVE_LEFT);
    input_handler.registerKeyboardKey(sf::Keyboard::Key::D, GameAction::MOVE_RIGHT);

    input_handler.registerKeyboardKey(sf::Keyboard::Key::Up, GameAction::MOVE_UP);
    input_handler.registerKeyboardKey(sf::Keyboard::Key::Down, GameAction::MOVE_DOWN);
    input_handler.registerKeyboardKey(sf::Keyboard::Key::Left, GameAction::MOVE_LEFT);
    input_handler.registerKeyboardKey(sf::Keyboard::Key::Right, GameAction::MOVE_RIGHT);

    input_handler.registerKeyboardKey(sf::Keyboard::Key::Space, GameAction::BOOST);
}



void TronClient::initControllerBindings()
{
    input_handler.registerControllerButton(InputHandler::XboxButton::A, GameAction::ACCEPT);
    input_handler.registerControllerButton(InputHandler::XboxButton::X, GameAction::BOOST);
    input_handler.registerControllerButton(InputHandler::XboxButton::B, GameAction::QUIT);
}



void TronClient::initClientStates()
{
    state_handler.registerState(STATE_LOBBY, std::make_unique<ClientStateLobby>(&client_data));
    state_handler.registerState(STATE_GAME, std::make_unique<ClientStateGame>(&client_data));
    state_handler.registerState(STATE_END, std::make_unique<ClientStateEnd>(&client_data));

    state_handler.queueState(STATE_LOBBY);
}



// Processes passed SFML events.
void TronClient::handleEvent(const sf::Event& _event)
{
    if (_event.type == sf::Event::Closed)
    {
        client_data.exit = true;
        window.close();

        return;
    }

    if (_event.type == sf::Event::GainedFocus)
    {
        in_focus = true;
    }
    else if (_event.type == sf::Event::LostFocus)
    {
        in_focus = false;
    }

    if (in_focus)
    {
        input_handler.handleEvent(_event);
    }
}



// Called by TronNetworkManager when a connection to the server has been made.
void TronClient::onConnected()
{
    postEvent([this]()
    {
        client_data.server_bulletin_str = "Connected";
    });
}



// Called by TronNetworkManager when the client becomes disconnected from the server.
void TronClient::onDisconnected()
{
    postEvent([this]()
    {
        state_handler.queueState(STATE_LOBBY);
        client_data.server_bulletin_str = "Disconnected";
    });
}



// Called by TronNetworkManager when the server replies to ping requests.
void TronClient::onUpdatePingTime(const double _ping)
{
    postEvent([this, _ping]()
    {
        client_data.latency = _ping;
    });
}



void TronClient::onIdentity(const unsigned int _player_id)
{
    postEvent([this, _player_id]()
    {
        client_data.client_id = _player_id;
        game_manager.addPlayer(_player_id);
    });
}



void TronClient::onPlayerList(const std::vector<Player>& _players)
{
    postEvent([this, _players]()
    {
        for (auto& player : _players)
        {
            game_manager.addPlayer(player.getID(), player.getState());
        }
    });
}



void TronClient::onPlayerJoined(const unsigned int _player_id)
{
    postEvent([this, _player_id]()
    {
        game_manager.addPlayer(_player_id);
    });
}



void TronClient::onPlayerLeft(const unsigned int _player_id)
{
    postEvent([this, _player_id]()
    {
        game_manager.removePlayer(_player_id);
    });
}



void TronClient::onPlayerStateChange(const unsigned int _player_id, const PlayerState _state)
{
    postEvent([this, _player_id, _state]()
    {
        game_manager.getPlayer(_player_id)->setState(_state);
    });
}



void TronClient::onGameStateChange(const int _state)
{
    postEvent([this, _state]()
    {
        state_handler.queueState(_state);
    });
}



void TronClient::onFlowControl(const FlowControl _control)
{
    postEvent([this, _control]()
    {
        switch (_control)
        {
            case FlowControl::COUNTDOWN:
            {
                game_manager.startCountdown();
            } break;

            case FlowControl::START:
            {
                game_audio.playSound(COUNTDOWN_FIN_CUE);
                game_audio.playMusic(GAME_MUSIC, 25.0f, true);

                game_manager.startSimulation();
            } break;

            case FlowControl::STOP:
            {
                game_audio.playSound(ROUND_OVER_CUE);
                game_audio.stopMusic();

                game_manager.stopSimulation();
            } break;

            default: {}
        }
    });
}



void TronClient::onVictor(const unsigned int _player_id)
{
    postEvent([this, _player_id]()
    {
        client_data.victor_id = _player_id;
    });
}



void TronClient::onBikeSync(const BikeState& _bike_state)
{
    postEvent([this, _bike_state]()
    {
        game_manager.getNetworkSimulation()->overwriteBike(_bike_state);
    });
}



void TronClient::onFullBikeSync(const std::array<BikeState, MAX_PLAYERS>& _bike_states)
{
    postEvent([this, _bike_states]()
    {
        game_manager.getNetworkSimulation()->overwriteBikes(_bike_states);
    });
}



void TronClient::onFullSync(const SimulationState& _simulation_state)
{
    postEvent([this, _simulation_state]()
    {
        game_manager.getNetworkSimulation()->overwrite(_simulation_state);
    });
}



void TronClient::onBikeRemoved(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        game_audio.playSound(DEATH_CUE);
        game_manager.getNetworkSimulation()->removeBike(_bike_id);
    });
}



void TronClient::onBikeBoost(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        game_audio.playSound(BOOST_CUE);
        game_manager.getNetworkSimulation()->boostBike(_bike_id);
    });
}



void TronClient::onBoostChargeGranted(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        // Don't bother player with recharging other bike boost charges.
        if (_bike_id == client_data.client_id)
        {
            game_audio.playSound(EXTRA_BOOST_CUE);
        }

        game_manager.getNetworkSimulation()->grantBoostCharge(_bike_id);
    });
}



void TronClient::onBulletinUpdate(const std::string& _bulletin)
{
    postEvent([this, _bulletin]()
    {
        client_data.server_bulletin_str = _bulletin;
    });
}



void TronClient::tick()
{
    updateServerReadout();

    game_manager.tick();
    game_audio.tick(client_data.delta_time);

    state_handler.tick();
}



void TronClient::draw()
{
    window.clear();

    state_handler.draw(window);
    window.draw(*server_readout);

    window.display();
}



void TronClient::updateServerReadout() const
{
    std::string server_str = SERVER_IP;
    server_str.append(":" + std::to_string(SERVER_TCP_PORT) + " \\ ping: ");

    std::string latency_str = std::to_string(client_data.latency);
    latency_str.erase(latency_str.find_first_of('.'), std::string::npos);
    latency_str.append("us");

    server_str.append(latency_str);

    server_readout->setString(server_str);
    JHelper::centerSFOrigin(*server_readout);
}

