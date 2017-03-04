#pragma once
#include <map>

#include "GameAction.h"

namespace sf
{
    class RenderWindow;
    class Event;
}

class TronGame;

class InputHandler
{
public:
    InputHandler(sf::RenderWindow& _window, TronGame& _tron_game);
    ~InputHandler() = default;

    bool handle_input(sf::Event& _event);
    void register_key(sf::Keyboard::Key _key, GameAction _game_action);

private:
    void check_bindings(sf::Event& _event);

    sf::RenderWindow& window;
    TronGame& tron_game;
    std::map<sf::Keyboard::Key, GameAction> key_bindings;

    int controllers_connected;

};
