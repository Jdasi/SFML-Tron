#pragma once
#include "ClientState.h"

namespace sf
{
    class Text;
}

/* Class representing the "End" state of the game.
 * I.e. the state in which the results of the last round are displayed.
 *
 * This state can only be triggered by the server when its simulation
 * has come to an end.
 */
class ClientStateEnd final : public ClientState
{
public:
    explicit ClientStateEnd(ClientData* _client_data);
    virtual ~ClientStateEnd() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

    void onCommand(const GameAction _action, const ActionState _action_state) override;

private:
    void initVictorText();
    void initBlackBar();

    sf::Text victor_text;
    sf::RectangleShape black_bar;

};
