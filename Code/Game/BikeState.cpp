#include "BikeState.h"

sf::Packet& operator<<(sf::Packet& _packet, const BikeState& _bike_state)
{
    _packet << static_cast<sf::Uint8>(_bike_state.id)
            << static_cast<sf::Uint8>(_bike_state.dir)
            << static_cast<sf::Uint32>(_bike_state.pos.x)
            << static_cast<sf::Uint32>(_bike_state.pos.y)
            << _bike_state.alive
            << _bike_state.boosting
            << static_cast<sf::Int32>(_bike_state.boost_charges)
            << _bike_state.boost_timer
            << _bike_state.extra_boost_timer;


    _packet << static_cast<sf::Uint32>(_bike_state.queued_moves.size());
    std::queue<Vector2i> queued_moves = _bike_state.queued_moves;
    while (!queued_moves.empty())
    {
        _packet << static_cast<sf::Uint32>(queued_moves.front().x)
                << static_cast<sf::Uint32>(queued_moves.front().y);

        queued_moves.pop();
    }
    
    _packet << static_cast<sf::Uint32>(_bike_state.line.size());
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
    bool        bike_alive;
    bool        bike_boosting;
    sf::Int32   boost_charges;
    double      boost_timer;
    double      extra_boost_timer;
    sf::Uint32  queued_moves_size;
    sf::Uint32  line_size;

    _bike_state.line.clear();

    _packet >> bike_id
            >> bike_dir
            >> bike_pos.x
            >> bike_pos.y
            >> bike_alive
            >> bike_boosting
            >> boost_charges
            >> boost_timer
            >> extra_boost_timer;

    _packet >> queued_moves_size;
    for (sf::Uint32 i = 0; i < queued_moves_size; ++i)
    {
        Vector2i move;

        _packet >> move.x >> move.y;
        _bike_state.queued_moves.push(move);
    }

    // Bike line is extracted last.
    _packet >> line_size;
    for (sf::Uint32 i = 0; i < line_size; ++i)
    {
        Vector2i pos;

        _packet >> pos.x >> pos.y;
        _bike_state.line.push_back(pos);
    }

    _bike_state.id = bike_id;
    _bike_state.dir = static_cast<MoveDirection>(bike_dir);
    _bike_state.pos = bike_pos;
    _bike_state.alive = bike_alive;
    _bike_state.boosting = bike_boosting;
    _bike_state.boost_timer = boost_timer;
    _bike_state.boost_charges = boost_charges;
    _bike_state.extra_boost_timer = extra_boost_timer;

    return _packet;
}
