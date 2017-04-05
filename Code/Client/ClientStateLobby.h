#pragma once
#include <Game/Scheduler.h>
#include "ClientState.h"
#include "LobbyUI.h"

/* Class representing the initial state of the game where users gather
 * to play the game.
 *
 * This state owns a UI which visualises the list of the players
 * recorded by the client's GameManager.
 * The UI is refreshed occassionally to ensure the information is up to date.
 *
 * Players can ready up in this state, and once enough players are ready
 * the server sends all clients in this state to the game state.
 */
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
    void initServerBulletinDisplay();
    void updateServerBulletinDisplay();
    void scheduleRefresh();

    sf::Text server_bulletin_display;

    Scheduler scheduler;
    LobbyUI lobby_ui;
    bool refresh_needed;

};
