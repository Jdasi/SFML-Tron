#pragma once
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "ClientState.h"

namespace sf
{
    class Text;
    class Drawable;
}

class ClientStateGame final : public ClientState
{
public:
    ClientStateGame(ClientData* _client_data);
    virtual ~ClientStateGame() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void onCommand(const GameAction _action, const ActionState _action_state) override;

private:
    sf::Text* title_text;

    std::vector<std::unique_ptr<sf::Drawable>> objects;

};
