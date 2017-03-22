#include <SFML/Graphics.hpp>

#include "ClientStateStart.h"
#include "ClientStateHandler.h"
#include "ClientData.h"
#include "TronNetworkManager.h"

ClientStateStart::ClientStateStart(ClientData* _client_data)
    : ClientState(_client_data)
{
    title_text = std::make_unique<sf::Text>("StateStart", *client_data->font);
    title_text->setCharacterSize(30);
    title_text->setStyle(sf::Text::Bold);
    title_text->setFillColor(sf::Color::Red);

    latency_text = std::make_unique<sf::Text>("", *client_data->font);
    latency_text->setCharacterSize(30);
    latency_text->setStyle(sf::Text::Bold);
    latency_text->setFillColor(sf::Color::Red);
    latency_text->setPosition({ 0, 50 });
}

void ClientStateStart::onStateEnter()
{
}

void ClientStateStart::onStateLeave()
{
}

void ClientStateStart::tick()
{
    latency_text->setString(std::to_string(client_data->latency) + "ms");
}

void ClientStateStart::draw(sf::RenderWindow& _window)
{
    _window.draw(*title_text);
    _window.draw(*latency_text);
}

void ClientStateStart::onCommand(const GameAction _action, const ActionState _action_state)
{
    if (_action == GameAction::ACCEPT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            getHandler()->queueState("GamePlay");
        }
    }

    if (_action == GameAction::INTERACT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->network_manager->sendChatMessage("Hello!");
        }
    }

    if (_action == GameAction::QUIT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->exit = true;
        }
    }
}
