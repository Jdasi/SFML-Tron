#include <SFML/Graphics.hpp>

#include "ClientStateStart.h"
#include "ClientStateHandler.h"
#include "ClientData.h"

ClientStateStart::ClientStateStart(ClientData* _client_data)
    : ClientState(_client_data)
{
    auto title_text = std::make_unique<sf::Text>("StateStart", *client_data->font);
    title_text->setCharacterSize(30);
    title_text->setStyle(sf::Text::Bold);
    title_text->setFillColor(sf::Color::Red);
    drawables.push_back(std::move(title_text));

    auto l_text = std::make_unique<sf::Text>("", *client_data->font);
    l_text->setCharacterSize(30);
    l_text->setStyle(sf::Text::Bold);
    l_text->setFillColor(sf::Color::Red);
    l_text->setPosition({ 0, 50 });
    latency_text = l_text.get();
    drawables.push_back(std::move(l_text));
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
    for (auto& drawable : drawables)
    {
        _window.draw(*drawable);
    }
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
