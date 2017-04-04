#pragma once
#include <SFML/Graphics.hpp>

#include "ClientState.h"
#include "Visualisation.h"

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
    void draw(sf::RenderWindow& _window) override;

    void onCommand(const GameAction _action, const ActionState _action_state) override;

private:
    void updateCountdownText();
    void handleBikeControls(const GameAction _action, const ActionState _action_state) const;

    Visualisation pretty_grid;
    std::unique_ptr<sf::Text> countdown_text;
    int last_tick_value;

};
