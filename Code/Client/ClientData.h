#pragma once
#include <atomic>

namespace sf
{
    class Font;
}

class InputHandler;
class ObjectRenderer;

struct ClientData
{
    ClientData(sf::Font* _font, InputHandler* _input_handler, ObjectRenderer* _object_renderer)
        : font(_font)
        , input_handler(_input_handler)
        , object_renderer(_object_renderer)
    {
    }

    sf::Font* font = nullptr;
    InputHandler* input_handler = nullptr;
    ObjectRenderer* object_renderer = nullptr;

    std::atomic<bool> exit = false;
    double delta_time = 0;
    double play_time = 0;
};
