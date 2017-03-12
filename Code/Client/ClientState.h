#pragma once
#include <vector>
#include <memory>

#include <Game/State.h>
#include "GameAction.h"

namespace sf
{
    class Drawable;
}

struct ClientData;
class ClientStateHandler;

/* The Client's extension of the generic State class.
 * ClientState requires a ClientStateHandler, which is derived from generic StateHandler.
 *
 * The extended state functionality includes command handling based on user input,
 * which the server does not need.
 */
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

    std::vector<std::unique_ptr<sf::Drawable>>* getDrawables()
    {
        return &drawables;
    }

protected:
    ClientData* client_data;
    std::vector<std::unique_ptr<sf::Drawable>> drawables;

private:


};
