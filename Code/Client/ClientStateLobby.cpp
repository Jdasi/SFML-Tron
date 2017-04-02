#include <SFML/Graphics.hpp>

#include <Game/Constants.h>
#include "ClientStateLobby.h"
#include "ClientStateHandler.h"
#include "ClientData.h"
#include "TronNetworkManager.h"
#include "AssetManager.h"

ClientStateLobby::ClientStateLobby(ClientData* _client_data)
    : ClientState(_client_data)
    , lobby_ui(_client_data)
    , refresh_needed(true)
{
    auto font = client_data->asset_manager->loadFont(DEFAULT_FONT);

    auto l_text = std::make_unique<sf::Text>("", *font);
    l_text->setCharacterSize(30);
    l_text->setStyle(sf::Text::Bold);
    l_text->setFillColor(sf::Color::Red);
    l_text->setPosition({ 0, 0 });
    latency_text = l_text.get();
    drawables.push_back(std::move(l_text));
}

void ClientStateLobby::onStateEnter()
{
    lobby_ui.refresh();
}

void ClientStateLobby::onStateLeave()
{
    refresh_needed = true;
}

void ClientStateLobby::tick()
{
    scheduler.update();

    scheduleRefresh();

    latency_text->setString(std::to_string(client_data->latency) + "ms");
}

void ClientStateLobby::draw(sf::RenderWindow& _window)
{
    lobby_ui.draw(_window);

    for (auto& drawable : drawables)
    {
        _window.draw(*drawable);
    }
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
            lobby_ui.refresh();
            refresh_needed = true;
        }, 0.5);
    }
}
