#include <Game/GameStateIDs.h>
#include <Game/JHelper.h>
#include "ClientStateGame.h"
#include "ClientData.h"
#include "ClientStateHandler.h"
#include "NetworkManager.h"
#include "GameManager.h"
#include "AssetManager.h"
#include "GameAudio.h"

ClientStateGame::ClientStateGame(ClientData* _client_data)
    : ClientState(_client_data)
    , pretty_grid(_client_data->asset_manager)
    , last_tick_value(0)
{
    client_data->game_manager->attachSimulationListener(&pretty_grid);

    countdown_text = std::make_unique<sf::Text>("",
        *client_data->asset_manager->loadFont(DEFAULT_FONT));

    countdown_text->setCharacterSize(60);
    countdown_text->setStyle(sf::Text::Bold);
    countdown_text->setPosition({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 });

    JHelper::centerSFOrigin(*countdown_text);
}

void ClientStateGame::onStateEnter()
{
    client_data->network_manager->sendPlayerStateChange(PlayerState::PLAYING);

    countdown_text->setFillColor(sf::Color::White);
    last_tick_value = static_cast<int>(COUNTDOWN_BEGIN);

    pretty_grid.updateBorderColor(
        JHelper::evaluateSFColorFromPlayerID(client_data->client_id));
}

void ClientStateGame::onStateLeave()
{
    client_data->game_audio->stopMusic();
    client_data->game_manager->resetSimulation();

    countdown_text->setString("");
}

void ClientStateGame::tick()
{
    updateCountdownText();

    pretty_grid.tick(client_data->delta_time);
}

void ClientStateGame::draw(sf::RenderWindow& _window)
{
    pretty_grid.draw(_window);

    _window.draw(*countdown_text);

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
            client_data->network_manager->sendPlayerStateChange(PlayerState::NOTREADY);
            getHandler()->queueState(STATE_LOBBY);
        }
    }

    if (client_data->game_manager->simulationRunning())
    {
        handleBikeControls(_action, _action_state);
    }
}



void ClientStateGame::updateCountdownText()
{
    int timer_value = client_data->game_manager->getCountdownDigit();
    countdown_text->setString(std::to_string(timer_value + 1));

    if (timer_value < last_tick_value)
    {
        client_data->game_audio->playSound(COUNTDOWN_TICK_CUE);
    }

    last_tick_value = timer_value;

    if (client_data->game_manager->simulationRunning())
    {
        countdown_text->setFillColor(sf::Color::Transparent);
    }
}



void ClientStateGame::handleBikeControls(const GameAction _action, 
    const ActionState _action_state) const
{
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
