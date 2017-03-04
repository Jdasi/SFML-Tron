#include <iostream>

#include <SFML/Graphics.hpp>

#include "InputHandler.h"
#include "TronClient.h"

// Pass in the client which the InputHandler is attached to.
InputHandler::InputHandler(TronClient& _attached_client)
    : tron_client(_attached_client)
    , controllers_connected(0)
{
}

bool InputHandler::handleInput(sf::Event& _event)
{
    bool event_handled = false;

    switch (_event.type)
    {
        case sf::Event::JoystickConnected:
        {
            sf::Joystick::Identification id = sf::Joystick::getIdentification(controllers_connected);
            ++controllers_connected;
        } break;

        case sf::Event::JoystickDisconnected:
        {
            --controllers_connected;
        } break;

        case sf::Event::JoystickMoved:
        {

        } break;

        case sf::Event::KeyPressed:
        {
            event_handled = true;
            checkKeyBindings(_event);
        } break;

        case sf::Event::KeyReleased:
        {
            event_handled = true;
            checkKeyBindings(_event);
        } break;

        case sf::Event::MouseButtonPressed:
        {
            event_handled = true;
            checkKeyBindings(_event);
        } break;

        case sf::Event::MouseButtonReleased:
        {
            event_handled = true;
            checkKeyBindings(_event);
        } break;

        case sf::Event::MouseMoved:
        {

        } break;

        case sf::Event::MouseWheelMoved:
        {
            event_handled = true;
            checkKeyBindings(_event);
        } break;

        case sf::Event::MouseWheelScrolled:
        {
            event_handled = true;
            checkKeyBindings(_event);
        } break;

        default: {}
    }

    return event_handled;
}

void InputHandler::registerKey(sf::Keyboard::Key _key, GameAction _game_action)
{
    // Don't do anything with already existing entries.
    auto entry = key_bindings.find(_key);
    if (entry != key_bindings.end())
    {
        return;
    }

    key_bindings.emplace(_key, _game_action);    
}

void InputHandler::checkKeyBindings(sf::Event& _event)
{
    // Determine the state of the TBD GameAction.
    ActionState action_state = ActionState::PRESSED;
    if (_event.type == sf::Event::KeyReleased)
    {
        action_state = ActionState::RELEASED;
    }

    // Only continue if the key code exists in the map.
    auto entry = key_bindings.find(_event.key.code);
    if (entry == key_bindings.end())
    {
        return;
    }

    // Inform tron client of the GameAction.
    tron_client.onCommand(entry->second, action_state);
}
