#include <SFML/Network.hpp>

#include "Bike.h"
#include "Constants.h"

Bike::Bike()
    : id(0)
    , direction(MoveDirection::RIGHT)
    , move_speed(BIKE_MOVE_SPEED)
    , move_timer(0)
    , alive(false)
    , boosting(false)
    , boost_timer(0)
    , boost_charges(STARTING_BOOST_CHARGES)
{
    line.reserve(GRID_AREA / 10);
}

void Bike::tick(double _dt)
{
    if (boost_timer > 0)
    {
        boost_timer -= _dt;
    }
    else
    {
        if (boosting)
        {
            boosting = false;
        }
    }
}

unsigned int Bike::getID() const
{
    return id;
}

void Bike::setID(const unsigned int _id)
{
    id = _id;
}

CellValue Bike::idToCellValue() const
{
    return static_cast<CellValue>(CellValue::CYAN + id);
}

MoveDirection Bike::getDirection() const
{
    return direction;
}

void Bike::setDirection(const MoveDirection _direction)
{
    direction = _direction;
}

Vector2i Bike::getPosition() const
{
    return pos;
}

void Bike::setPosition(const Vector2i& _pos)
{
    pos = _pos;

    line.push_back(_pos);
}

const std::vector<Vector2i>& Bike::getLine() const
{
    return line;
}

void Bike::setLine(const std::vector<Vector2i>& _line)
{
    line = _line;
}

float Bike::getMoveSpeed() const
{
    if (boosting)
    {
        return move_speed / BIKE_BOOST_DIVISOR;
    }
    
    return move_speed;
}

void Bike::setMoveSpeed(const float _speed)
{
    move_speed = _speed;
}

double Bike::getMoveTimer() const
{
    return move_timer;
}

void Bike::setMoveTimer(const double _value)
{
    move_timer = _value;
}

void Bike::resetMoveTimer()
{
    move_timer = 0;
}

void Bike::modifyMoveTimer(const double _dt)
{
    move_timer += _dt;
}

bool Bike::isAlive() const
{
    return alive;
}

void Bike::setAlive(const bool _value)
{
    alive = _value;
}

bool Bike::isBoosting() const
{
    return boosting;
}

bool Bike::boost()
{
    if (!alive || boosting || boost_charges <= 0)
    {
        return false;
    }

    boosting = true;
    boost_timer = BIKE_BOOST_DURATION;
    --boost_charges;

    return true;
}

sf::Packet& operator<<(sf::Packet& _packet, Bike& _bike)
{
    _packet << static_cast<sf::Uint8>(_bike.id)
            << static_cast<sf::Uint8>(_bike.direction)
            << static_cast<sf::Uint32>(_bike.pos.x)
            << static_cast<sf::Uint32>(_bike.pos.y)
            << _bike.move_timer
            << _bike.alive
            << _bike.boosting
            << _bike.boost_timer
            << static_cast<sf::Int32>(_bike.boost_charges)
            << static_cast<sf::Uint32>(_bike.line.size());

    for (auto& pos : _bike.line)
    {
        _packet << static_cast<sf::Uint32>(pos.x) << static_cast<sf::Uint32>(pos.y);
    }

    return _packet;
}

sf::Packet& operator>>(sf::Packet& _packet, Bike& _bike)
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

    _bike.line.clear();

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

    _bike.id                = bike_id;
    _bike.direction         = static_cast<MoveDirection>(bike_dir);
    _bike.pos               = bike_pos;
    _bike.move_timer        = move_timer;
    _bike.alive             = bike_alive;
    _bike.boosting          = bike_boosting;
    _bike.boost_timer       = boost_timer;
    _bike.boost_charges     = boost_charges;

    for (sf::Uint32 i = 0; i < line_length; ++i)
    {
        Vector2i pos;

        _packet >> pos.x >> pos.y;
        _bike.line.push_back(pos);
    }

    return _packet;
}
