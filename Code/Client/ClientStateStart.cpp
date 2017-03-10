#include <SFML/Graphics.hpp>

#include "ClientStateStart.h"
#include "ClientStateHandler.h"
#include "ClientData.h"
#include "ObjectRenderer.h"

ClientStateStart::ClientStateStart(ClientData* _client_data)
    : ClientState(_client_data)
    , title_text(nullptr)
{
}

void ClientStateStart::onStateEnter()
{
    // Create text.
    auto text = std::make_unique<sf::Text>("StateStart", *client_data->font);
    text->setCharacterSize(30);
    text->setStyle(sf::Text::Bold);
    text->setFillColor(sf::Color::Red);

    title_text = text.get();
    objects.push_back(std::move(text));

    for (auto& obj : objects)
    {
        client_data->object_renderer->link(*obj);
    }
}

void ClientStateStart::onStateLeave()
{
    for (auto& obj : objects)
    {
        client_data->object_renderer->unlink(*obj);
    }
}

void ClientStateStart::tick()
{
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

    if (_action == GameAction::QUIT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->exit = true;
        }
    }
}
