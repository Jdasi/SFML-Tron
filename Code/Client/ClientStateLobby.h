#pragma once
#include <Game/Scheduler.h>
#include "ClientState.h"
#include "LobbyUI.h"

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
    void scheduleRefresh();

    sf::Text* latency_text;
    Scheduler scheduler;

    LobbyUI lobby_ui;
    bool refresh_needed;

};
