#include <SFML/Graphics.hpp>

#include <Game/Constants.h>
#include "InputHandler.h"
#include "TronClient.h"

// Pass in the client which the InputHandler is attached to.
InputHandler::InputHandler(TronClient& _attached_client)
    : tron_client(_attached_client)
    , joystick_x(0)
    , joystick_y(0)
    , prev_joystick_x(0)
    , prev_joystick_y(0)
{
}



/* Handles input-based SFML events.
 * This function assumes that the window is in focus and should be taking input.
 *
 * Not all input-events are currently handled. The function returns true if
 * the event was handled, otherwise it returns false.
 */
bool InputHandler::handleEvent(const sf::Event& _event)
{
    bool event_handled = false;

    switch (_event.type)
    {
        case sf::Event::JoystickMoved:
        {
            joystick_x = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
            joystick_y = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);

            handleJoystickMovement();
            event_handled = true;

            break;
        }

        case sf::Event::JoystickButtonPressed:
        {
            checkControllerBindings(_event);
            event_handled = true;

            break;
        }

        case sf::Event::KeyPressed:
        {
            checkKeyboardBindings(_event);
            event_handled = true;

            break;
        }

        case sf::Event::KeyReleased:
        {
            checkKeyboardBindings(_event);
            event_handled = true;

            break;
        }

        default: {}
    }

    return event_handled;
}



// Register a unique Keyboard key with a GameAction.
void InputHandler::registerKeyboardKey(const sf::Keyboard::Key _key,
    const GameAction _game_action)
{
    // Don't do anything with already existing entries.
    auto entry = keyboard_bindings.find(_key);
    if (entry != keyboard_bindings.end())
    {
        return;
    }

    keyboard_bindings.emplace(_key, _game_action);
}



// Register a unique Controller button with a GameAction.
void InputHandler::registerControllerButton(const unsigned int _button,
    const GameAction _game_action)
{
    // Don't do anything with already existing entries.
    auto entry = controller_bindings.find(_button);
    if (entry != controller_bindings.end())
    {
        return;
    }

    controller_bindings.emplace(_button, _game_action);
}



// Translates joystick movements into GameActions.
void InputHandler::handleJoystickMovement()
{
    if (axisFirstPushed(joystick_y, prev_joystick_y, -AXIS_DEADZONE))
    {
        tron_client.onCommand(GameAction::MOVE_UP, ActionState::PRESSED);
    }

    if (axisFirstPushed(joystick_y, prev_joystick_y, AXIS_DEADZONE))
    {
        tron_client.onCommand(GameAction::MOVE_DOWN, ActionState::PRESSED);
    }

    if (axisFirstPushed(joystick_x, prev_joystick_x, -AXIS_DEADZONE))
    {
        tron_client.onCommand(GameAction::MOVE_LEFT, ActionState::PRESSED);
    }

    if (axisFirstPushed(joystick_x, prev_joystick_x, AXIS_DEADZONE))
    {
        tron_client.onCommand(GameAction::MOVE_RIGHT, ActionState::PRESSED);
    }

    // Update previous positions once all checks have been made.
    prev_joystick_x = joystick_x;
    prev_joystick_y = joystick_y;
}



/* Helper function to determine when the joystick was first pushed 
 * in a certain direction, using the previous variables of the joystick.
 */
bool InputHandler::axisFirstPushed(const float _axis, const float _prev_axis,
    const float _deadzone) const
{
    if (_deadzone < 0)
    {
        if (_axis <= _deadzone && _prev_axis > _deadzone)
        {
            return true;
        }
    }
    else
    {
        if (_axis >= _deadzone && _prev_axis < _deadzone)
        {
            return true;
        }
    }

    return false;
}



/* Informs the client of the identified GameAction if one is mapped
 * to the passed event's key code.
 */
void InputHandler::checkKeyboardBindings(const sf::Event& _event)
{
    // Determine the state of the TBD GameAction.
    ActionState action_state = ActionState::PRESSED;
    if (_event.type == sf::Event::KeyReleased)
    {
        action_state = ActionState::RELEASED;
    }

    auto entry = keyboard_bindings.find(_event.key.code);
    if (entry != keyboard_bindings.end())
    {
        tron_client.onCommand(entry->second, action_state);
    }
}



/* Informs the client of the identified GameAction if one is mapped
 * to the passed event's button code.
 */
void InputHandler::checkControllerBindings(const sf::Event& _event)
{
    ActionState action_state = ActionState::PRESSED;
    if (_event.type == sf::Event::JoystickButtonReleased)
    {
        action_state = ActionState::RELEASED;
    }

    auto entry = controller_bindings.find(_event.joystickButton.button);
    if (entry != controller_bindings.end())
    {
        tron_client.onCommand(entry->second, action_state);
    }
}
