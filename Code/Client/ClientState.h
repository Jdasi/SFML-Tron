#pragma once
#include <Game/State.h>
#include "GameAction.h"

struct GameData;
class ClientStateHandler;

class ClientState : public State<ClientState, ClientStateHandler>
{
public:
    explicit ClientState(GameData& _game_data)
        : game_data(_game_data)
    {
    }
    
    ~ClientState() = default;

    virtual void onCommand(GameAction _action, ActionState _action_state) = 0;

    void onStateEnter() override = 0;
    void onStateLeave() override = 0;
    void tick() override = 0;

protected:
    GameData& game_data;

private:


};
