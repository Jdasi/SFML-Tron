#include "ClientStateGame.h"
#include "ClientData.h"
#include "ClientStateHandler.h"

ClientStateGame::ClientStateGame(ClientData* _client_data)
    : ClientState(_client_data)
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
}

void ClientStateGame::onStateLeave()
{
}

void ClientStateGame::tick()
{
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
            simulation.changePlayerDirection(client_data->client_id, MoveDirection::UP);
        }
    }

    if (_action == GameAction::MOVE_DOWN)
    {
        if (_action_state == ActionState::PRESSED)
        {
            simulation.changePlayerDirection(client_data->client_id, MoveDirection::DOWN);
        }
    }

    if (_action == GameAction::MOVE_LEFT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            simulation.changePlayerDirection(client_data->client_id, MoveDirection::LEFT);
        }
    }

    if (_action == GameAction::MOVE_RIGHT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            simulation.changePlayerDirection(client_data->client_id, MoveDirection::RIGHT);
        }
    }
}
