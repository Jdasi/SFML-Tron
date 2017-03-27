#pragma once
#include <atomic>

namespace sf
{
    class Font;
}

class InputHandler;
class ObjectFactory;
class TronNetworkManager;
class GameManager;

struct ClientData
{
    ClientData(sf::Font* _font, InputHandler* _input_handler, 
        TronNetworkManager* _network_manager, GameManager* _game_manager)
        : client_id(0)
        , font(_font)
        , input_handler(_input_handler)
        , network_manager(_network_manager)
        , game_manager(_game_manager)
        , exit(false)
        , delta_time(0)
        , play_time(0)
        , latency(0)
    {
    }

    int client_id;
    sf::Font* font;
    InputHandler* input_handler;
    TronNetworkManager* network_manager;
    GameManager* game_manager;

    std::atomic<bool> exit;
    double delta_time;
    double play_time;

    double latency;

};
