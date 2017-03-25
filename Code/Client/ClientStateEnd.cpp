#include <SFML/Graphics.hpp>

#include "ClientStateEnd.h"
#include "ClientStateHandler.h"
#include "ClientData.h"
#include "TronNetworkManager.h"

ClientStateEnd::ClientStateEnd(ClientData* _client_data)
    : ClientState(_client_data)
{
    auto title_text = std::make_unique<sf::Text>("StateEnd", *client_data->font);
    title_text->setCharacterSize(30);
    title_text->setStyle(sf::Text::Bold);
    title_text->setFillColor(sf::Color::Red);
    drawables.push_back(std::move(title_text));
}

void ClientStateEnd::onStateEnter()
{
}

void ClientStateEnd::onStateLeave()
{
}

void ClientStateEnd::tick()
{
}

void ClientStateEnd::draw(sf::RenderWindow& _window)
{
    for (auto& drawable : drawables)
    {
        _window.draw(*drawable);
    }
}

void ClientStateEnd::onCommand(const GameAction _action, const ActionState _action_state)
{
    if (_action == GameAction::QUIT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->exit = true;
        }
    }
}