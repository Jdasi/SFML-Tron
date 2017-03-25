#pragma once
#include <SFML/Graphics.hpp>

#include <Game/Simulation.h>
#include "ClientState.h"
#include "PrettyGrid.h"

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
    PrettyGrid pretty_grid;
    Simulation simulation;

};
