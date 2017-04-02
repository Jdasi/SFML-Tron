#pragma once
#include <map>

#include "GameAction.h"

namespace sf
{
    class RenderWindow;
    class Event;
}

class TronClient;

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

    explicit InputHandler(TronClient& _attached_client);
    ~InputHandler() = default;

    bool handleEvent(const sf::Event& _event);
    void registerKeyboardKey(const sf::Keyboard::Key _key, const GameAction _game_action);
    void registerControllerButton(const unsigned int _button, const GameAction _game_action);

private:
    void handleJoystickMovement() const;
    bool axisFirstPushed(const float _axis, const float _prev_axis,
        const float _deadzone) const;

    void checkKeyboardBindings(const sf::Event& _event);
    void checkControllerBindings(const sf::Event& _event);

    TronClient& tron_client;
    std::map<sf::Keyboard::Key, GameAction> keyboard_bindings;
    std::map<unsigned int, GameAction> controller_bindings;

    float joystick_x;
    float joystick_y;

    float prev_joystick_x;
    float prev_joystick_y;

};
