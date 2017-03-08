#pragma once
#include <Game/StateHandler.h>
#include "GameAction.h"
#include "ClientState.h"

class ClientStateHandler : public StateHandler<ClientState, ClientStateHandler>
{
public:
    ClientStateHandler() = default;
    virtual ~ClientStateHandler() = default;

    void onCommand(GameAction _action, ActionState _action_state)
    {
        if (current_state)
        {
            current_state->onCommand(_action, _action_state);
        }
    }

private:


};
