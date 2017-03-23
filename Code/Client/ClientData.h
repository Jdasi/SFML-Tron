#pragma once
#include <atomic>

namespace sf
{
    class Font;
}

class InputHandler;
class ObjectFactory;
class TronNetworkManager;

struct ClientData
{
    ClientData(sf::Font* _font, InputHandler* _input_handler, TronNetworkManager* _network_manager)
        : font(_font)
        , input_handler(_input_handler)
        , network_manager(_network_manager)
    {
    }

    int client_id = 0;
    sf::Font* font = nullptr;
    InputHandler* input_handler = nullptr;
    TronNetworkManager* network_manager = nullptr;

    std::atomic<bool> exit = false;
    double delta_time = 0;
    double play_time = 0;

    sf::Uint32 latency = 0;

};
