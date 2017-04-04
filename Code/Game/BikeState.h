#pragma once
#include <vector>

#include <SFML/Network.hpp>

#include "Constants.h"
#include "Vector2i.h"
#include "MoveDirection.h"

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



    friend sf::Packet& operator<<(sf::Packet& _packet, BikeState _bike_state)
    {
        _packet << static_cast<sf::Uint8>(_bike_state.id)
                << static_cast<sf::Uint8>(_bike_state.dir)
                << static_cast<sf::Uint32>(_bike_state.pos.x)
                << static_cast<sf::Uint32>(_bike_state.pos.y)
                << _bike_state.move_timer
                << _bike_state.alive
                << _bike_state.boosting
                << _bike_state.boost_timer
                << static_cast<sf::Int32>(_bike_state.boost_charges)
                << static_cast<sf::Uint32>(_bike_state.line.size());

        for (auto& pos : _bike_state.line)
        {
            _packet << static_cast<sf::Uint32>(pos.x) << static_cast<sf::Uint32>(pos.y);
        }

        return _packet;
    }



    friend sf::Packet& operator>>(sf::Packet& _packet, BikeState& _bike_state)
    {
        sf::Uint8   bike_id;
        sf::Uint8   bike_dir;
        Vector2i    bike_pos;
        double      move_timer;
        bool        bike_alive;
        bool        bike_boosting;
        double      boost_timer;
        sf::Int32   boost_charges;
        sf::Uint32  line_length;

        _bike_state.line.clear();

        _packet >> bike_id
                >> bike_dir
                >> bike_pos.x
                >> bike_pos.y
                >> move_timer
                >> bike_alive
                >> bike_boosting
                >> boost_timer
                >> boost_charges
                >> line_length;

        _bike_state.id              = bike_id;
        _bike_state.dir             = static_cast<MoveDirection>(bike_dir);
        _bike_state.pos             = bike_pos;
        _bike_state.move_timer      = move_timer;
        _bike_state.alive           = bike_alive;
        _bike_state.boosting        = bike_boosting;
        _bike_state.boost_timer     = boost_timer;
        _bike_state.boost_charges   = boost_charges;

        for (sf::Uint32 i = 0; i < line_length; ++i)
        {
            Vector2i pos;

            _packet >> pos.x >> pos.y;
            _bike_state.line.push_back(pos);
        }

        return _packet;
    }
};
