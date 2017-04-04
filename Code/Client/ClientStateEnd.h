#pragma once
#include "ClientState.h"

namespace sf
{
    class Text;
    class Drawable;
}

class ClientStateEnd final : public ClientState
{
public:
    ClientStateEnd(ClientData* _client_data);
    virtual ~ClientStateEnd() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

    void onCommand(const GameAction _action, const ActionState _action_state) override;

private:
    std::unique_ptr<sf::Text> victor_text;

};
