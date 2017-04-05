#pragma once
#include <vector>

#include <SFML/Network.hpp>

#include "Constants.h"
#include "Vector2i.h"
#include "MoveDirection.h"

/* Struct containing all of the data a bike uses that can be 
 * synchronised across the server.
 *
 * Operator overloads are provided to allow the BikeState to be
 * read into and out of a packet.
 */
struct BikeState
{
    unsigned int id             = 0;
    MoveDirection dir           = MoveDirection::RIGHT;
    Vector2i pos                ;
    std::vector<Vector2i> line  ;
    float move_speed            = BIKE_MOVE_SPEED;
    double move_timer           = 0;
    bool alive                  = false;
    bool boosting               = false;
    double boost_timer          = 0;
    unsigned int boost_charges  = STARTING_BOOST_CHARGES;
    double extra_boost_timer    = 0; // Server handles this, no need to transfer.

    friend sf::Packet& operator<<(sf::Packet& _packet,
        const BikeState& _bike_state);

    friend sf::Packet& operator>>(sf::Packet& _packet,
        BikeState& _bike_state);

};
