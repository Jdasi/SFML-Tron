#include <Game/Simulation.h>
#include <Game/GameStateIDs.h>
#include "ClientStateGame.h"
#include "ClientData.h"
#include "ClientStateHandler.h"
#include "TronNetworkManager.h"
#include "GameManager.h"
#include "AssetManager.h"

ClientStateGame::ClientStateGame(ClientData* _client_data)
    : ClientState(_client_data)
    , pretty_grid(_client_data->asset_manager)
{
    client_data->game_manager->getSimulation()->attachListener(&pretty_grid);

    auto title_text = std::make_unique<sf::Text>("StateGame",
        *client_data->asset_manager->loadFontTTF("arial"));

    title_text->setCharacterSize(30);
    title_text->setStyle(sf::Text::Bold);
    title_text->setFillColor(sf::Color::Red);
    drawables.push_back(std::move(title_text));
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
    auto id = client_data->client_id;

    if (_action == GameAction::QUIT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            getHandler()->queueState(STATE_LOBBY);
        }
    }

    if (_action == GameAction::MOVE_UP)
    {
        if (_action_state == ActionState::PRESSED)
        {
            auto dir = MoveDirection::UP;

            client_data->network_manager->sendBikeDirectionChange(id, dir);
        }
    }

    if (_action == GameAction::MOVE_DOWN)
    {
        if (_action_state == ActionState::PRESSED)
        {
            auto dir = MoveDirection::DOWN;

            client_data->network_manager->sendBikeDirectionChange(id, dir);
        }
    }

    if (_action == GameAction::MOVE_LEFT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            auto dir = MoveDirection::LEFT;

            client_data->network_manager->sendBikeDirectionChange(id, dir);
        }
    }

    if (_action == GameAction::MOVE_RIGHT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            auto dir = MoveDirection::RIGHT;

            client_data->network_manager->sendBikeDirectionChange(id, dir);
        }
    }
}
