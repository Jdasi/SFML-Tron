#pragma once
#include <Game/StateHandler.h>
#include "GameAction.h"
#include "ClientState.h"
#include "ObjectRenderer.h"

/* The Client's extension of the generic StateHandler class.
 * ClientStateHandler handles ClientStates, which are derived from the generic State.
 *
 * The extended handler functionality includes command handling based on user input,
 * which the server does not need.
 */
class ClientStateHandler final : public StateHandler<ClientState, ClientStateHandler>
{
public:
    ClientStateHandler(ObjectRenderer* _object_renderer)
        : object_renderer(_object_renderer)
    {
    }
    
    virtual ~ClientStateHandler() = default;

    void onCommand(GameAction _action, ActionState _action_state) const
    {
        if (current_state)
        {
            current_state->onCommand(_action, _action_state);
        }
    }

private:
    void onStateLeave(ClientState* _state) override
    {
        object_renderer->link(nullptr);
    }
    
    void onStateEnter(ClientState* _state) override
    {
        object_renderer->link(_state->getDrawables());
    }

    ObjectRenderer* object_renderer;

};
