#include "Player.h"

Player::Player()
    : id(0)
    , state(NOTREADY)
    , is_client(false)
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

Player::State Player::getState() const
{
    return state;
}

void Player::setState(State _state)
{
    state = _state;
}

bool Player::isClient() const
{
    return is_client;
}

void Player::setClient(bool _value)
{
    is_client = _value;
}
