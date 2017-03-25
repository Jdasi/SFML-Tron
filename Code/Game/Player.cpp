#include "Player.h"
#include "Constants.h"

Player::Player()
    : id(0)
    , colour(CellColour::CYAN)
    , direction(MoveDirection::RIGHT)
    , move_speed(BIKE_MOVE_SPEED)
    , move_timer(0)
    , alive(true)
    , boosting(false)
{
}

int Player::getID() const
{
    return id;
}

void Player::setID(int _id)
{
    id = _id;
}

CellColour Player::getColour() const
{
    return colour;
}

void Player::setColour(CellColour _colour)
{
    colour = _colour;
}

MoveDirection Player::getDirection() const
{
    return direction;
}

void Player::setDirection(MoveDirection _direction)
{
    direction = _direction;
}

Vector2i Player::getPosition() const
{
    return pos;
}

void Player::setPosition(const Vector2i& _pos)
{
    pos = _pos;
}

float Player::getMoveSpeed() const
{
    return move_speed;
}

void Player::setMoveSpeed(float _speed)
{
    move_speed = _speed;
}

double Player::getMoveTimer() const
{
    return move_timer;
}

void Player::resetMoveTimer()
{
    move_timer = 0;
}

void Player::modifyMoveTimer(double _dt)
{
    move_timer += _dt;
}

bool Player::isAlive() const
{
    return alive;
}

void Player::setAlive(bool _value)
{
    alive = _value;
}

bool Player::isBoosting() const
{
    return boosting;
}

void Player::setBoosting(bool _value)
{
    boosting = _value;
}
