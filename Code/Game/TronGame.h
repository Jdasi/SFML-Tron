#pragma once
#include <memory>

#include "InputHandler.h"

namespace sf
{
	class Packet;
}

class TronGame
{
public:
	TronGame();
	~TronGame() = default;

    void run();

	friend sf::Packet& operator <<(sf::Packet& packet, const TronGame& game);
	friend sf::Packet& operator >>(sf::Packet& packet, const TronGame& game);

private:
    std::unique_ptr<InputHandler> input_handler_;

};
