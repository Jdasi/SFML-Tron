#include <SFML/Graphics.hpp>

#include "ClientStateStart.h"
#include "ClientStateHandler.h"
#include "ClientData.h"
#include "ObjectRenderer.h"
#include "TronNetworkManager.h"

ClientStateStart::ClientStateStart(ClientData* _client_data)
    : ClientState(_client_data)
{
    // Create text.
    auto ttext = std::make_unique<sf::Text>("StateStart", *client_data->font);
    ttext->setCharacterSize(30);
    ttext->setStyle(sf::Text::Bold);
    ttext->setFillColor(sf::Color::Red);

    title_text = ttext.get();
    drawables.push_back(std::move(ttext));

    // Create text.
    auto ltext = std::make_unique<sf::Text>("", *client_data->font);
    ltext->setCharacterSize(30);
    ltext->setStyle(sf::Text::Bold);
    ltext->setFillColor(sf::Color::Red);
    ltext->setPosition({ 0, 50 });

    latency_text = ltext.get();
    drawables.push_back(std::move(ltext));
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
