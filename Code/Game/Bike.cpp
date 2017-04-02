#include "Bike.h"
#include "Constants.h"

Bike::Bike()
{
    state.line.reserve(GRID_AREA / 10);
}

void Bike::tick(double _dt)
{
    if (state.boost_timer > 0)
    {
        state.boost_timer -= _dt;
    }
    else
    {
        if (state.boosting)
        {
            state.boosting = false;
        }
    }
}

unsigned int Bike::getID() const
{
    return state.id;
}

void Bike::setID(const unsigned int _id)
{
    state.id = _id;
}

BikeState Bike::getState() const
{
    return state;
}

void Bike::overwriteState(const BikeState& _state)
{
    state = _state;
}

MoveDirection Bike::getDirection() const
{
    return state.dir;
}

void Bike::setDirection(const MoveDirection _direction)
{
    state.dir = _direction;
}

Vector2i Bike::getPosition() const
{
    return state.pos;
}

void Bike::setPosition(const Vector2i& _pos)
{
    state.pos = _pos;

    state.line.push_back(_pos);
}

const std::vector<Vector2i>& Bike::getLine() const
{
    return state.line;
}

void Bike::setLine(const std::vector<Vector2i>& _line)
{
    state.line = _line;
}

float Bike::getMoveSpeed() const
{
    if (state.boosting)
    {
        return state.move_speed / BIKE_BOOST_DIVISOR;
    }
    
    return state.move_speed;
}

void Bike::setMoveSpeed(const float _speed)
{
    state.move_speed = _speed;
}

double Bike::getMoveTimer() const
{
    return state.move_timer;
}

void Bike::setMoveTimer(const double _value)
{
    state.move_timer = _value;
}

void Bike::resetMoveTimer()
{
    state.move_timer = 0;
}

void Bike::modifyMoveTimer(const double _dt)
{
    state.move_timer += _dt;
}

bool Bike::isAlive() const
{
    return state.alive;
}

void Bike::setAlive(const bool _value)
{
    state.alive = _value;
}

bool Bike::isBoosting() const
{
    return state.boosting;
}

bool Bike::boost()
{
    if (!state.alive || state.boosting || state.boost_charges <= 0)
    {
        return false;
    }

    state.boosting = true;
    state.boost_timer = BIKE_BOOST_DURATION;
    --state.boost_charges;

    return true;
}
