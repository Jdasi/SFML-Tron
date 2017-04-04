#include <SFML/Graphics.hpp>

#include <Game/Constants.h>
#include <Game/JHelper.h>
#include "ClientStateLobby.h"
#include "ClientStateHandler.h"
#include "ClientData.h"
#include "NetworkManager.h"
#include "AssetManager.h"
#include "GameManager.h"
#include "GameAudio.h"

ClientStateLobby::ClientStateLobby(ClientData* _client_data)
    : ClientState(_client_data)
    , lobby_ui(_client_data)
    , refresh_needed(true)
{
    auto backdrop = std::make_unique<sf::Sprite>(
        *client_data->asset_manager->loadTexture(BACKDROP));
    drawables.push_back(std::move(backdrop));

    server_bulletin = std::make_unique<sf::Text>("",
        *client_data->asset_manager->loadFont(DEFAULT_FONT));

    server_bulletin->setCharacterSize(40);
    server_bulletin->setStyle(sf::Text::Bold);
    server_bulletin->setFillColor(sf::Color::White);
    server_bulletin->setPosition({ WINDOW_WIDTH / 2, WINDOW_HEIGHT * 0.1f });
    server_bulletin->setOutlineColor(sf::Color::Black);
    server_bulletin->setOutlineThickness(2.0f);
}

void ClientStateLobby::onStateEnter()
{
    updateServerBulletin();

    client_data->game_manager->resetSimulation();

    lobby_ui.refresh();
}

void ClientStateLobby::onStateLeave()
{
    client_data->game_audio->stopMusic();
    refresh_needed = true;
}

void ClientStateLobby::tick()
{
    scheduler.update();

    scheduleRefresh();
}

void ClientStateLobby::draw(sf::RenderWindow& _window)
{
    for (auto& drawable : drawables)
    {
        _window.draw(*drawable);
    }

    _window.draw(*server_bulletin);
    lobby_ui.draw(_window);
}

void ClientStateLobby::onCommand(const GameAction _action, const ActionState _action_state)
{
    if (_action == GameAction::QUIT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->exit = true;
        }
    }

    if (_action == GameAction::ACCEPT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->network_manager->sendPlayerStateChange();
        }
    }
}

void ClientStateLobby::scheduleRefresh()
{
    if (refresh_needed)
    {
        refresh_needed = false;

        scheduler.invoke([this]()
        {
            updateServerBulletin();
            lobby_ui.refresh();

            refresh_needed = true;
        }, 0.5);
    }
}



void ClientStateLobby::updateServerBulletin() const
{
    server_bulletin->setString(client_data->server_bulletin_str);
    JHelper::centerSFOrigin(*server_bulletin);
}
