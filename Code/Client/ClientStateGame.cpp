#include <SFML/Graphics.hpp>

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
    , visualisation(_client_data)
    , last_tick_value(0)
{
    // The Visualisation must be attached to the simulation to function correctly.
    client_data->game_manager->attachSimulationListener(&visualisation);

    initCountdownDisplay();
}



void ClientStateGame::onStateEnter()
{
    // Inform the server the client has loaded in.
    client_data->network_manager->sendPlayerStateChange(PlayerState::PLAYING);

    resetCountdownDisplay();

    // The client doesn't start off knowing its ID.
    visualisation.updateClientColor();
}



void ClientStateGame::onStateLeave()
{
    client_data->game_audio->stopMusic();
    client_data->game_manager->resetSimulation();

    countdown_display.setString("");
}



void ClientStateGame::tick()
{
    updateCountdownDisplay();

    visualisation.tick(client_data->delta_time);
}



void ClientStateGame::draw(sf::RenderWindow& _window)
{
    visualisation.draw(_window);

    _window.draw(countdown_display);

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
            // Inform the server the client has gone back to lobby.
            client_data->network_manager->sendPlayerStateChange(PlayerState::NOTREADY);
            getHandler()->queueState(STATE_LOBBY);
        }
    }

    if (client_data->game_manager->simulationRunning())
    {
        handleBikeControls(_action, _action_state);
    }
}



void ClientStateGame::initCountdownDisplay()
{
    countdown_display.setFont(*client_data->asset_manager->loadFont(DEFAULT_FONT));
    countdown_display.setCharacterSize(60);
    countdown_display.setStyle(sf::Text::Bold);
    countdown_display.setPosition({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 });

    JHelper::centerSFOrigin(countdown_display);
}



void ClientStateGame::resetCountdownDisplay()
{
    countdown_display.setFillColor(sf::Color::White);
    last_tick_value = static_cast<int>(COUNTDOWN_BEGIN);
}



void ClientStateGame::updateCountdownDisplay()
{
    int timer_value = client_data->game_manager->getCountdownDigit();
    countdown_display.setString(std::to_string(timer_value + 1));

    if (timer_value < last_tick_value)
    {
        client_data->game_audio->playSound(COUNTDOWN_TICK_CUE);
    }

    last_tick_value = timer_value;

    if (client_data->game_manager->simulationRunning())
    {
        countdown_display.setFillColor(sf::Color::Transparent);
    }
}



void ClientStateGame::handleBikeControls(const GameAction _action, 
    const ActionState _action_state) const
{
    if (_action == GameAction::BOOST)
    {
        if (_action_state == ActionState::PRESSED)
        {
            return client_data->network_manager->sendBikeBoost();
        }
    }

    if (_action == GameAction::MOVE_UP)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->network_manager->sendBikeDirectionChange(MoveDirection::UP);
        }
    }

    if (_action == GameAction::MOVE_DOWN)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->network_manager->sendBikeDirectionChange(MoveDirection::DOWN);
        }
    }

    if (_action == GameAction::MOVE_LEFT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->network_manager->sendBikeDirectionChange(MoveDirection::LEFT);
        }
    }

    if (_action == GameAction::MOVE_RIGHT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->network_manager->sendBikeDirectionChange(MoveDirection::RIGHT);
        }
    }
}
