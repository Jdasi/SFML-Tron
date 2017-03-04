#include <iostream>

#include <SFML/Graphics.hpp>

#include "InputHandler.h"

InputHandler::InputHandler(sf::RenderWindow& _window, TronGame& _tron_game)
    : window(_window)
    , tron_game(_tron_game)
    , controllers_connected(0)
{
}

bool InputHandler::handle_input(sf::Event& _event)
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

        } break;

        case sf::Event::JoystickMoved:
        {

        } break;

        case sf::Event::KeyPressed:
        {
            event_handled = true;
            check_bindings(_event);
        } break;

        case sf::Event::KeyReleased:
        {
            event_handled = true;
            check_bindings(_event);
        } break;

        case sf::Event::MouseButtonPressed:
        {
            event_handled = true;
            check_bindings(_event);
        } break;

        case sf::Event::MouseButtonReleased:
        {
            event_handled = true;
            check_bindings(_event);
        } break;

        case sf::Event::MouseMoved:
        {

        } break;

        case sf::Event::MouseWheelMoved:
        {
            event_handled = true;
            check_bindings(_event);
        } break;

        case sf::Event::MouseWheelScrolled:
        {
            event_handled = true;
            check_bindings(_event);
        } break;

        default: {}
    }

    return event_handled;
}

void InputHandler::register_key(sf::Keyboard::Key _key, GameAction _game_action)
{
    auto entry = key_bindings.find(_key);
    if (entry != key_bindings.end())
    {
        return;
    }

    key_bindings.emplace(_key, _game_action);    
}

void InputHandler::check_bindings(sf::Event& _event)
{
    auto entry = key_bindings.find(_event.key.code);
    if (entry == key_bindings.end())
    {
        return;
    }

    switch (entry->second)
    {
        case GameAction::QUIT:
        {
            window.close();
        } break;

        default: {}
    }
}
