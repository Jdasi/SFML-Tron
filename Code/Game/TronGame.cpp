#include <SFML/Network.hpp>

#include "TronGame.h"

sf::Packet& operator<<(sf::Packet& _packet, const TronGame& _game)
{
	return _packet;
}

sf::Packet& operator>>(sf::Packet& _packet, const TronGame& _game)
{
	return _packet;
}
