#include "BikeState.h"

sf::Packet& operator<<(sf::Packet& _packet, const BikeState& _bike_state)
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

    // Bike line is read in last.
    for (auto& pos : _bike_state.line)
    {
        _packet << static_cast<sf::Uint32>(pos.x) << static_cast<sf::Uint32>(pos.y);
    }

    return _packet;
}



sf::Packet& operator>>(sf::Packet& _packet, BikeState& _bike_state)
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

    // Bine line is extracted last.
    for (sf::Uint32 i = 0; i < line_length; ++i)
    {
        Vector2i pos;

        _packet >> pos.x >> pos.y;
        _bike_state.line.push_back(pos);
    }

    return _packet;
}
