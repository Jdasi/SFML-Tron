#pragma once
#include "ClientState.h"

class ClientStateGame : public ClientState
{
public:
    ClientStateGame() = default;
    virtual ~ClientStateGame() = default;

    void onCommand(GameAction _action, ActionState _action_state) override {}
    void onStateEnter() override {}
    void onStateLeave() override {}
    void tick() override {}

protected:


private:


};
