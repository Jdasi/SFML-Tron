#include <SFML/Network.hpp>

#include "TronGame.h"

sf::Packet& operator<<(sf::Packet& packet, const TronGame& game)
{
	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, const TronGame& game)
{
	return packet;
}
