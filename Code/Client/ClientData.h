#pragma once
#include <atomic>

namespace sf
{
    class Font;
}

class InputHandler;
class ObjectFactory;
class TronNetworkManager;
class Simulation;

struct ClientData
{
    ClientData(sf::Font* _font, InputHandler* _input_handler, 
        TronNetworkManager* _network_manager, Simulation* _simulation)
        : client_id(0)
        , font(_font)
        , input_handler(_input_handler)
        , network_manager(_network_manager)
        , simulation(_simulation)
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
    Simulation* simulation;

    std::atomic<bool> exit;
    double delta_time;
    double play_time;

    sf::Uint32 latency;

};
