#pragma once
#include "ClientState.h"
#include "Visualisation.h"

namespace sf
{
    class Text;
}

/* Class in which the visualisation of the client's simulation takes place.
 *  
 * This state owns the Visualisation, which listens to the GameManager's
 * simulation on the client side and updates its visuals accordingly.
 *
 * This is the only state in which the bikes can be controlled, and this state
 * can only be triggered by the server when its simulation is ready to start.
 */
class ClientStateGame final : public ClientState
{
public:
    explicit ClientStateGame(ClientData* _client_data);
    virtual ~ClientStateGame() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

    void onCommand(const GameAction _action, const ActionState _action_state) override;

private:
    void initCountdownDisplay();
    void resetCountdownDisplay();
    void updateCountdownDisplay();

    void handleBikeControls(const GameAction _action, const ActionState _action_state) const;

    Visualisation visualisation;
    sf::Text countdown_display;
    int last_tick_value;

};
