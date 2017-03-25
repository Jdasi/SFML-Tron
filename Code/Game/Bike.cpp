#include "Bike.h"
#include "Constants.h"

Bike::Bike()
    : id(0)
    , colour(CellColour::CYAN)
    , direction(MoveDirection::RIGHT)
    , move_speed(BIKE_MOVE_SPEED)
    , move_timer(0)
    , alive(true)
    , boosting(false)
{
}

int Bike::getID() const
{
    return id;
}

void Bike::setID(int _id)
{
    id = _id;
}

CellColour Bike::getColour() const
{
    return colour;
}

void Bike::setColour(CellColour _colour)
{
    colour = _colour;
}

MoveDirection Bike::getDirection() const
{
    return direction;
}

void Bike::setDirection(MoveDirection _direction)
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
}

float Bike::getMoveSpeed() const
{
    return move_speed;
}

void Bike::setMoveSpeed(float _speed)
{
    move_speed = _speed;
}

double Bike::getMoveTimer() const
{
    return move_timer;
}

void Bike::resetMoveTimer()
{
    move_timer = 0;
}

void Bike::modifyMoveTimer(double _dt)
{
    move_timer += _dt;
}

bool Bike::isAlive() const
{
    return alive;
}

void Bike::setAlive(bool _value)
{
    alive = _value;
}

bool Bike::isBoosting() const
{
    return boosting;
}

void Bike::setBoosting(bool _value)
{
    boosting = _value;
}
