#include "ClientStateGame.h"
#include "ClientData.h"
#include "ObjectRenderer.h"
#include "ClientStateHandler.h"

ClientStateGame::ClientStateGame(ClientData* _client_data)
    : ClientState(_client_data)
{
    // Create text.
    auto ttext = std::make_unique<sf::Text>("StateGame", *client_data->font);
    ttext->setCharacterSize(30);
    ttext->setStyle(sf::Text::Bold);
    ttext->setFillColor(sf::Color::Red);

    title_text = ttext.get();
    drawables.push_back(std::move(ttext));
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
