#include "ClientStateHandler.h"

void ClientStateHandler::onCommand(GameAction _action, ActionState _action_state) const
{
    if (current_state)
    {
        current_state->onCommand(_action, _action_state);
    }
}



void ClientStateHandler::draw(sf::RenderWindow& _window) const
{
    if (current_state)
    {
        current_state->draw(_window);
    }
}



ClientState* ClientStateHandler::getCurrentState() const
{
    return current_state;
}
