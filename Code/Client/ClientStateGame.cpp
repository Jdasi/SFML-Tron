#include <Game/Constants.h>
#include "ClientStateGame.h"
#include "ClientData.h"
#include "ClientStateHandler.h"
#include "TronNetworkManager.h"

#include <iostream>

ClientStateGame::ClientStateGame(ClientData* _client_data)
    : ClientState(_client_data)
    , network_manager(*this, SERVER_IP, SERVER_TCP_PORT)
{
    simulation.attachListener(&pretty_grid);

    auto title_text = std::make_unique<sf::Text>("StateGame", *client_data->font);
    title_text->setCharacterSize(30);
    title_text->setStyle(sf::Text::Bold);
    title_text->setFillColor(sf::Color::Red);
    drawables.push_back(std::move(title_text));

    simulation.addPlayer();
}

void ClientStateGame::onStateEnter()
{
    network_manager.connect();
}

void ClientStateGame::onStateLeave()
{
}

void ClientStateGame::tick()
{
    executeDispatchedMethods();

    simulation.tick(client_data->delta_time);
}

void ClientStateGame::draw(sf::RenderWindow& _window)
{
    pretty_grid.draw(_window);

    for (auto& drawable : drawables)
    {
        _window.draw(*drawable);
    }
}

void ClientStateGame::onCommand(const GameAction _action, const ActionState _action_state)
{
    auto id = client_data->client_id;

    if (_action == GameAction::QUIT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            getHandler()->queueState("GameStart");
        }
    }

    if (_action == GameAction::MOVE_UP)
    {
        if (_action_state == ActionState::PRESSED)
        {
            auto dir = MoveDirection::UP;

            simulation.changePlayerDirection(id, dir);
            network_manager.sendPlayerDirectionChange(id, dir);
        }
    }

    if (_action == GameAction::MOVE_DOWN)
    {
        if (_action_state == ActionState::PRESSED)
        {
            auto dir = MoveDirection::DOWN;

            simulation.changePlayerDirection(id, dir);
            network_manager.sendPlayerDirectionChange(id, dir);
        }
    }

    if (_action == GameAction::MOVE_LEFT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            auto dir = MoveDirection::LEFT;

            simulation.changePlayerDirection(id, dir);
            network_manager.sendPlayerDirectionChange(id, dir);
        }
    }

    if (_action == GameAction::MOVE_RIGHT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            auto dir = MoveDirection::RIGHT;

            simulation.changePlayerDirection(id, dir);
            network_manager.sendPlayerDirectionChange(id, dir);
        }
    }
}

// Called by TronNetworkManager when a connection to the server has been made.
void ClientStateGame::onConnected()
{
    postEvent([this]()
    {
        std::cout << "Connected." << std::endl;
    });
}

// Called by TronNetworkManager when the client becomes disconnected from the server.
void ClientStateGame::onDisconnected()
{
    postEvent([this]()
    {
        std::cout << "Disconnected." << std::endl;
    });
}

// Called by TronNetworkManager when the server replies to ping requests.
void ClientStateGame::onUpdatePingTime(const sf::Uint32 _ping)
{
    postEvent([this, _ping]()
    {
        client_data->latency = _ping;
        std::cout << "Ping: " << _ping << std::endl;
    });
}

void ClientStateGame::onPlayerDirectionChange(int _id, MoveDirection _dir)
{
    postEvent([this, _id, _dir]()
    {
        simulation.changePlayerDirection(_id, _dir);
    });
}
