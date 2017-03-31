#include <SFML/Graphics.hpp>

#include <Game/PlayerState.h>
#include <Game/Constants.h>
#include "ClientStateLobby.h"
#include "ClientStateHandler.h"
#include "ClientData.h"
#include "TronNetworkManager.h"
#include "AssetManager.h"

ClientStateLobby::ClientStateLobby(ClientData* _client_data)
    : ClientState(_client_data)
    , ready(false)
{
    auto* font = client_data->asset_manager->loadFont(DEFAULT_FONT);

    auto title_text = std::make_unique<sf::Text>("StateLobby", *font);
    title_text->setCharacterSize(30);
    title_text->setStyle(sf::Text::Bold);
    title_text->setFillColor(sf::Color::Red);
    drawables.push_back(std::move(title_text));

    auto l_text = std::make_unique<sf::Text>("", *font);
    l_text->setCharacterSize(30);
    l_text->setStyle(sf::Text::Bold);
    l_text->setFillColor(sf::Color::Red);
    l_text->setPosition({ 0, 50 });
    latency_text = l_text.get();
    drawables.push_back(std::move(l_text));
}

void ClientStateLobby::onStateEnter()
{
    client_data->network_manager->connect();
}

void ClientStateLobby::onStateLeave()
{
}

void ClientStateLobby::tick()
{
    latency_text->setString(std::to_string(client_data->latency) + "ms");
}

void ClientStateLobby::draw(sf::RenderWindow& _window)
{
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
            ready = !ready;

            PlayerState state = ready ? PlayerState::READY : PlayerState::NOTREADY;
            client_data->network_manager->sendPlayerStateChange(client_data->client_id, state);
        }
    }
}
