#pragma once
#include <Game/StateHandler.h>
#include "GameAction.h"
#include "ClientState.h"

namespace sf 
{
    class RenderWindow;
}

/* The Client's extension of the generic StateHandler class.
 * ClientStateHandler handles ClientStates, which are derived from the generic State.
 *
 * The extended handler functionality includes command handling based on user input,
 * which the server does not need.
 */
class ClientStateHandler final : public StateHandler<ClientState, ClientStateHandler>
{
public:
    ClientStateHandler() = default;    
    virtual ~ClientStateHandler() = default;

    void onCommand(GameAction _action, ActionState _action_state) const;

    void draw(sf::RenderWindow& _window) const;

private:
    void onStateLeave(ClientState* _state) override {}
    void onStateEnter(ClientState* _state) override {}

};
