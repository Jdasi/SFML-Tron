#pragma once
#include "ClientState.h"

namespace sf
{
    class Text;
    class Drawable;
}

class ClientStateLobby final : public ClientState
{
public:
    ClientStateLobby(ClientData* _client_data);
    virtual ~ClientStateLobby() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

    void onCommand(const GameAction _action, const ActionState _action_state) override;

private:


};
