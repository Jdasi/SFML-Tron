#pragma once
#include <vector>
#include <memory>

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
    void onCommand(const GameAction _action, const ActionState _action_state) override;

private:
    sf::Text* title_text;
    sf::Text* latency_text;

};
