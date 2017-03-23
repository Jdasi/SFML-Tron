#pragma once
#include <vector>
#include <memory>
#include <functional>

#include <SFML/Graphics.hpp>

#include "ClientState.h"
#include "GameGrid.h"
#include "BikeManager.h"

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
    GameGrid game_grid;
    BikeManager grid_controller;

};
