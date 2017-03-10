#pragma once
#include <Game/State.h>
#include "GameAction.h"

struct ClientData;
class ClientStateHandler;

class ClientState : public State<ClientState, ClientStateHandler>
{
public:
    explicit ClientState(ClientData* _client_data)
        : client_data(_client_data)
    {
    }
    
    ~ClientState() = default;

    virtual void onCommand(const GameAction _action, const ActionState _action_state) = 0;

    void onStateEnter() override = 0;
    void onStateLeave() override = 0;

    void tick() override = 0;

protected:
    ClientData* client_data;

private:


};
