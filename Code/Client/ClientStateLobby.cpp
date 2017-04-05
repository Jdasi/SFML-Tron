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

    initServerBulletinDisplay();
}



void ClientStateLobby::onStateEnter()
{
    updateServerBulletinDisplay();

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

    _window.draw(server_bulletin_display);
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

    if (_action == GameAction::ACCEPT ||
        _action == GameAction::BOOST)
    {
        if (_action_state == ActionState::PRESSED)
        {
            // Inform the server the client is/isn't ready.
            client_data->network_manager->sendPlayerStateChange();
        }
    }
}



void ClientStateLobby::initServerBulletinDisplay()
{
    server_bulletin_display.setFont(
        *client_data->asset_manager->loadFont(DEFAULT_FONT));

    server_bulletin_display.setCharacterSize(40);
    server_bulletin_display.setStyle(sf::Text::Bold);
    server_bulletin_display.setFillColor(sf::Color::White);
    server_bulletin_display.setPosition({ WINDOW_WIDTH / 2, WINDOW_HEIGHT * 0.1f });
    server_bulletin_display.setOutlineColor(sf::Color::Black);
    server_bulletin_display.setOutlineThickness(2.0f);
}



void ClientStateLobby::updateServerBulletinDisplay()
{
    server_bulletin_display.setString(client_data->server_bulletin);
    JHelper::centerSFOrigin(server_bulletin_display);
}



// Schedules a refresh of the LobbyUI every 0.5 sec if one is needed.
void ClientStateLobby::scheduleRefresh()
{
    if (refresh_needed)
    {
        refresh_needed = false;

        scheduler.invoke([this]()
        {
            updateServerBulletinDisplay();
            lobby_ui.refresh();

            refresh_needed = true;
        }, 0.5);
    }
}
