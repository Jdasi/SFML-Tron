#pragma once
#include <map>

#include "GameAction.h"

namespace sf
{
    class RenderWindow;
    class Event;
}

class TronClient;

/* Class for translating passed SFML events into GameActions.
 * The InputHandler only processes inputs if its in_focus bool is true.
 *
 * Lists of unique bindings are checked to determine if the key or button code
 * translates into a GameAction.
 *
 * The same key cannot be registered with the InputHandler more than once.
 */
class InputHandler final
{
public:
    enum XboxButton
    {
        A = 0,
        B = 1,
        X = 2,
        Y = 3
    };

    explicit InputHandler(TronClient& _attached_client, bool& _in_focus);
    ~InputHandler() = default;

    bool handleEvent(const sf::Event& _event);
    void registerKeyboardKey(const sf::Keyboard::Key _key, const GameAction _game_action);
    void registerControllerButton(const XboxButton _button, const GameAction _game_action);

private:
    void handleJoystickMovement();
    bool axisFirstPushed(const float _axis, const float _prev_axis,
        const float _deadzone) const;

    void checkKeyboardBindings(const sf::Event& _event);
    void checkControllerBindings(const sf::Event& _event);

    TronClient& tron_client;
    bool& in_focus;

    std::map<sf::Keyboard::Key, GameAction> keyboard_bindings;
    std::map<unsigned int, GameAction> controller_bindings;

    float joystick_x;
    float joystick_y;

    float prev_joystick_x;
    float prev_joystick_y;

};
