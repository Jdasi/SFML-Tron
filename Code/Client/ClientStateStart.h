#pragma once
#include "ClientState.h"

class ClientStateStart : public ClientState
{
public:
    ClientStateStart() = default;
    virtual ~ClientStateStart() = default;

    void onCommand(GameAction _action, ActionState _action_state) override {}
    void onStateEnter() override {}
    void onStateLeave() override {}
    void tick() override {}

protected:


private:


};
