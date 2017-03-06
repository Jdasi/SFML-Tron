#pragma once
#include <SFML/Network.hpp>

enum PacketID
{
    DISCONNECT,
    PING,
    PONG,
    LATENCY,
    MESSAGE
};

// This function must be called after creating a packet to set its type.
inline void setPacketID(sf::Packet& _packet, PacketID _id)
{
    _packet << static_cast<sf::Uint8>(_id);
}

// This function must be called after receiving a packet to determine its type.
inline PacketID getPacketID(sf::Packet& _packet)
{
    sf::Uint8 id;
    _packet >> id;
    return static_cast<PacketID>(id);
}
