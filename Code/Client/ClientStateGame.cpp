#include "ClientStateGame.h"
#include "ClientData.h"
#include "ClientStateHandler.h"

ClientStateGame::ClientStateGame(ClientData* _client_data)
    : ClientState(_client_data)
{
    title_text = std::make_unique<sf::Text>("StateGame", *client_data->font);
    title_text->setCharacterSize(30);
    title_text->setStyle(sf::Text::Bold);
    title_text->setFillColor(sf::Color::Red);
}

void ClientStateGame::onStateEnter()
{
}

void ClientStateGame::onStateLeave()
{
}

void ClientStateGame::tick()
{
    
}

void ClientStateGame::draw(sf::RenderWindow& _window)
{
    _window.draw(*title_text);
}

void ClientStateGame::onCommand(const GameAction _action, const ActionState _action_state)
{
    if (_action == GameAction::QUIT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            getHandler()->queueState("GameStart");
        }
    }
}
