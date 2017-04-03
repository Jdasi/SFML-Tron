#include <Game/GameStateIDs.h>
#include "ClientStateGame.h"
#include "ClientData.h"
#include "ClientStateHandler.h"
#include "NetworkManager.h"
#include "GameManager.h"
#include "AssetManager.h"

ClientStateGame::ClientStateGame(ClientData* _client_data)
    : ClientState(_client_data)
    , pretty_grid(_client_data->asset_manager)
{
    client_data->game_manager->attachSimulationListener(&pretty_grid);
}

void ClientStateGame::onStateEnter()
{
}

void ClientStateGame::onStateLeave()
{
}

void ClientStateGame::tick()
{
    pretty_grid.tick(client_data->delta_time);
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
    if (_action == GameAction::QUIT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->network_manager->sendPlayerStateChange();
            getHandler()->queueState(STATE_LOBBY);
        }
    }

    if (_action == GameAction::MOVE_UP)
    {
        if (_action_state == ActionState::PRESSED)
        {
            auto dir = MoveDirection::UP;

            client_data->network_manager->sendBikeDirectionChange(dir);
        }
    }

    if (_action == GameAction::MOVE_DOWN)
    {
        if (_action_state == ActionState::PRESSED)
        {
            auto dir = MoveDirection::DOWN;

            client_data->network_manager->sendBikeDirectionChange(dir);
        }
    }

    if (_action == GameAction::MOVE_LEFT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            auto dir = MoveDirection::LEFT;

            client_data->network_manager->sendBikeDirectionChange(dir);
        }
    }

    if (_action == GameAction::MOVE_RIGHT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            auto dir = MoveDirection::RIGHT;

            client_data->network_manager->sendBikeDirectionChange(dir);
        }
    }

    if (_action == GameAction::BOOST)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->network_manager->sendBikeBoost();
        }
    }
}
