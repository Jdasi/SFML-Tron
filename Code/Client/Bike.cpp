#include "Bike.h"
#include <Game/Constants.h>

Bike::Bike()
    : bike_id(0)
    , trail_color(sf::Color::Transparent)
    , grid_position({ 0, 0 })
    , move_direction(MoveDirection::RIGHT)
    , move_speed(BIKE_MOVE_SPEED)
    , move_timer(0.0f)
    , alive(true)
{
}

int Bike::getID() const
{
    return bike_id;
}

void Bike::setID(int _id)
{
    bike_id = _id;
}

sf::Color Bike::getTrailColor() const
{
    return trail_color;
}

void Bike::setTrailColor(sf::Color _color)
{
    trail_color = _color;
}

sf::Vector2<int> Bike::getGridPosition() const
{
    return grid_position;
}

void Bike::setGridPosition(sf::Vector2<int> _grid_pos)
{
    grid_position = _grid_pos;
}

MoveDirection Bike::getMoveDirection() const
{
    return move_direction;
}

void Bike::setMoveDirection(MoveDirection _direction)
{
    move_direction = _direction;
}

float Bike::getMoveSpeed() const
{
    return move_speed;
}

void Bike::setMoveSpeed(float _value)
{
    move_speed = _value;
}

double Bike::getMoveTimer() const
{
    return move_timer;
}

void Bike::resetMoveTimer()
{
    move_timer = 0;
}

void Bike::modifyMoveTimer(double _value)
{
    move_timer += _value;
}

bool Bike::isAlive() const
{
    return alive;
}

void Bike::setAlive(bool _value)
{
    alive = _value;
}
