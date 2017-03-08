#pragma once

namespace sf
{
	class Packet;
}

class TronGame
{
public:
	TronGame() = default;
	~TronGame() = default;

	friend sf::Packet& operator<<(sf::Packet& _packet, const TronGame& _game);
	friend sf::Packet& operator>>(sf::Packet& _packet, const TronGame& _game);

private:

};
