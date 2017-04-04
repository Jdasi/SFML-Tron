#pragma once
#include <Game/Scheduler.h>
#include "ClientState.h"
#include "LobbyUI.h"

class ClientStateLobby final : public ClientState
{
public:
    explicit ClientStateLobby(ClientData* _client_data);
    virtual ~ClientStateLobby() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

    void onCommand(const GameAction _action, const ActionState _action_state) override;

private:
    void initServerBulletin();
    void updateServerBulletin();
    void scheduleRefresh();

    sf::Text server_bulletin;

    Scheduler scheduler;
    LobbyUI lobby_ui;
    bool refresh_needed;

};
