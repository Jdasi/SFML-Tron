#include <SFML/Graphics.hpp>

#include <Game/Constants.h>
#include "ClientStateLobby.h"
#include "ClientStateHandler.h"
#include "ClientData.h"
#include "NetworkManager.h"
#include "AssetManager.h"
#include "GameManager.h"

ClientStateLobby::ClientStateLobby(ClientData* _client_data)
    : ClientState(_client_data)
    , lobby_ui(_client_data)
    , refresh_needed(true)
{
    auto font = client_data->asset_manager->loadFont(DEFAULT_FONT);

    auto backdrop = std::make_unique<sf::Sprite>(
        *client_data->asset_manager->loadTexture(BACKDROP));
    drawables.push_back(std::move(backdrop));

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
    client_data->game_manager->resetSimulation();
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

    auto str = std::to_string(client_data->latency);
    str.erase(str.find_first_of('.'), std::string::npos);

    latency_text->setString(str + "us");
}

void ClientStateLobby::draw(sf::RenderWindow& _window)
{
    for (auto& drawable : drawables)
    {
        _window.draw(*drawable);
    }

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
            lobby_ui.refresh();
            refresh_needed = true;
        }, 0.5);
    }
}
