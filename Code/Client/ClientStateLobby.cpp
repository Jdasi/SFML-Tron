#include <SFML/Graphics.hpp>

#include "ClientStateLobby.h"
#include "ClientStateHandler.h"
#include "ClientData.h"
#include "TronNetworkManager.h"

ClientStateLobby::ClientStateLobby(ClientData* _client_data)
    : ClientState(_client_data)
{
    auto title_text = std::make_unique<sf::Text>("StateLobby", *client_data->font);
    title_text->setCharacterSize(30);
    title_text->setStyle(sf::Text::Bold);
    title_text->setFillColor(sf::Color::Red);
    drawables.push_back(std::move(title_text));
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
}
