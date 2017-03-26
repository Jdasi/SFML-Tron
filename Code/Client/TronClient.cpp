#include <iostream>

#include <SFML/Graphics.hpp>

#include <Game/Constants.h>
#include <Game/GameStateIDs.h>
#include "TronClient.h"
#include "ClientStateLobby.h"
#include "ClientStateGame.h"
#include "ClientStateEnd.h"

TronClient::TronClient()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tron Game")
    , network_manager(*this, SERVER_IP, SERVER_TCP_PORT)
    , game_manager(&client_data)
    , input_handler(*this)
    , state_handler()
    , client_data(&font, &input_handler, &network_manager, game_manager.getSimulation())
{
    if (!font.loadFromFile("../../Resources/arial.ttf"))
    {
        throw std::runtime_error("Font not found");
    }
}

void TronClient::run()
{
    initKeyBindings();
    initClientStates();

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

    window.close();
}

// Forwards any input-based commands to the current state for processing.
void TronClient::onCommand(GameAction _action, ActionState _action_state) const
{
    state_handler.onCommand(_action, _action_state);
}

void TronClient::initKeyBindings()
{
    input_handler.registerKey(sf::Keyboard::Key::Escape, GameAction::QUIT);
    input_handler.registerKey(sf::Keyboard::Key::Return, GameAction::ACCEPT);
    input_handler.registerKey(sf::Keyboard::Key::W, GameAction::MOVE_UP);
    input_handler.registerKey(sf::Keyboard::Key::S, GameAction::MOVE_DOWN);
    input_handler.registerKey(sf::Keyboard::Key::A, GameAction::MOVE_LEFT);
    input_handler.registerKey(sf::Keyboard::Key::D, GameAction::MOVE_RIGHT);
    input_handler.registerKey(sf::Keyboard::Key::E, GameAction::INTERACT);
}

void TronClient::initClientStates()
{
    state_handler.registerState(STATE_LOBBY, std::make_unique<ClientStateLobby>(&client_data));
    state_handler.registerState(STATE_GAME, std::make_unique<ClientStateGame>(&client_data));
    state_handler.registerState(STATE_END, std::make_unique<ClientStateEnd>(&client_data));

    state_handler.queueState(STATE_LOBBY);
}

// Processes the passed SFML event.
void TronClient::handleEvent(const sf::Event& _event)
{
    // Input Handler gets first dibs at handling events.
    if (input_handler.handleInput(_event))
    {
        return;
    }

    if (_event.type == sf::Event::Closed)
    {
        client_data.exit = true;
        window.close();
    }
}

// Called by TronNetworkManager when a connection to the server has been made.
void TronClient::onConnected()
{
    postEvent([this]()
    {
        std::cout << "Connected." << std::endl;
    });
}

// Called by TronNetworkManager when the client becomes disconnected from the server.
void TronClient::onDisconnected()
{
    postEvent([this]()
    {
        std::cout << "Disconnected." << std::endl;
    });
}

// Called by TronNetworkManager when the server replies to ping requests.
void TronClient::onUpdatePingTime(double _ping)
{
    postEvent([this, _ping]()
    {
        client_data.latency = _ping;
        std::cout << "Ping: " << _ping << std::endl;
    });
}

// Called by TronNetworkManager when the server sends a player direction change.
void TronClient::onBikeDirectionChange(int _id, MoveDirection _dir)
{
    postEvent([this, _id, _dir]()
    {
        game_manager.getNetworkSimulation()->changeBikeDirection(_id, _dir);
    });
}

void TronClient::onIdentity(int _id)
{
    postEvent([this, _id]()
    {
        client_data.client_id = _id;
        game_manager.addPlayer(_id);

        std::cout << "Identity assigned: " << _id << std::endl;
    });
}

void TronClient::onPlayerList(std::vector<Player> _players)
{
    postEvent([this, _players]()
    {
        for (auto& player : _players)
        {
            game_manager.addPlayer(player.getID(), player.getState());
        }
    });
}

void TronClient::onPlayerJoined(int _id)
{
    postEvent([this, _id]()
    {
        game_manager.addPlayer(_id);
    });
}

void TronClient::onPlayerStateChange(int _player_id, PlayerState _state)
{
    postEvent([this, _player_id, _state]()
    {
        game_manager.getPlayer(_player_id)->setState(_state);
    });
}

void TronClient::onGameStateChange(int _state)
{
    postEvent([this, _state]()
    {
        switch (_state)
        {
            case STATE_LOBBY:
            {

            } break;

            case STATE_GAME:
            {
                game_manager.startSimulation();
            } break;

            case STATE_END:
            {
                game_manager.stopSimulation();
            } break;

            default: {}
        }

        state_handler.queueState(_state);
    });
}

void TronClient::tick()
{
    game_manager.tick();
    state_handler.tick();
}

void TronClient::draw()
{
    window.clear();

    state_handler.draw(window);

    window.display();
}
