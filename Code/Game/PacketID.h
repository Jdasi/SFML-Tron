#pragma once
#include <SFML/Network.hpp>

// The types of messages that can be sent over the network.
enum class PacketID
{
        DISCONNECT,
        PING,
        PONG,
        IDENTITY,
        PLAYER_LIST,
        PLAYER_JOINED,
        PLAYER_LEFT,
        PLAYER_STATE,
        GAME_STATE,
        FLOW_CONTROL,
        VICTOR,
        MESSAGE,
        BULLETIN,
        BIKE_DIRECTION,
        BIKE_REMOVED,
        BIKE_BOOST,
        EXTRA_BOOST,
        SYNC_BIKE,
        SYNC_ALL_BIKES,
        SYNC_SIMULATION
};



// This function must be called after creating a packet to set its type.
inline void setPacketID(sf::Packet& _packet, PacketID _id)
{
    _packet.clear();
    _packet << static_cast<sf::Uint8>(_id);
}



// This function must be called after receiving a packet to determine its type.
inline PacketID getPacketID(sf::Packet& _packet)
{
    sf::Uint8 id;
    _packet >> id;
    return static_cast<PacketID>(id);
}
