#pragma once
#include <SFML/Graphics.hpp>

#include <Game/Simulation.h>
#include "ClientState.h"
#include "PrettyGrid.h"
#include "NetworkClient.h"
#include "ThreadDispatcher.h"
#include "TronNetworkManager.h"

namespace sf
{
    class Text;
    class Drawable;
}

class ClientStateGame final : public ClientState, public INetworkClient, public ThreadDispatcher
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
    void onConnected() override;
    void onDisconnected() override;
    void onUpdatePingTime(const sf::Uint32 _ping) override;
    void onPlayerDirectionChange(int _id, MoveDirection _dir) override;

    TronNetworkManager network_manager;
    PrettyGrid pretty_grid;
    Simulation simulation;

};
