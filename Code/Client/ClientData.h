#pragma once
#include <atomic>

namespace sf
{
    class Font;
}

class InputHandler;
class ObjectRenderer;
class TronNetworkManager;

struct ClientData
{
    ClientData(sf::Font* _font, InputHandler* _input_handler, ObjectRenderer* _object_renderer, TronNetworkManager* _network_manager)
        : font(_font)
        , input_handler(_input_handler)
        , object_renderer(_object_renderer)
        , network_manager(_network_manager)
    {
    }

    sf::Font* font = nullptr;
    InputHandler* input_handler = nullptr;
    ObjectRenderer* object_renderer = nullptr;
    TronNetworkManager* network_manager = nullptr;

    std::atomic<bool> exit = false;
    double delta_time = 0;
    double play_time = 0;

    sf::Uint32 latency = 0;

};
