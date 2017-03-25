#pragma once
#include "ClientState.h"

namespace sf
{
    class Text;
    class Drawable;
}

class ClientStateStart final : public ClientState
{
public:
    ClientStateStart(ClientData* _client_data);
    virtual ~ClientStateStart() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

    void onCommand(const GameAction _action, const ActionState _action_state) override;

private:
    sf::Text* latency_text;

};
