#pragma once
#include <map>

#include "GameAction.h"

namespace sf
{
    class RenderWindow;
    class Event;
}

class TronClient;

class InputHandler
{
public:
    explicit InputHandler(TronClient& _attached_client);
    ~InputHandler() = default;

    bool handleInput(const sf::Event& _event);
    void registerKey(sf::Keyboard::Key _key, GameAction _game_action);

private:
    void checkBindings(const sf::Event& _event);

    TronClient& tron_client;
    std::map<sf::Keyboard::Key, GameAction> key_bindings;

    int controllers_connected;

};
