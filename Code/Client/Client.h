#pragma once
#include <memory>

#include <SFML/Graphics.hpp>

#include <Game/TronGame.h>
#include "InputHandler.h"

class Client
{
public:
    Client();
    ~Client() = default;

    void run();

private:
    void handle_event(sf::Event& _event) const;

    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<TronGame> tron_game;
    std::unique_ptr<InputHandler> input_handler;

};
