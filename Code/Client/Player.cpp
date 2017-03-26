#include "Player.h"

Player::Player(int _id, PlayerState _state)
    : id(_id)
    , state(_state)
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

PlayerState Player::getState() const
{
    return state;
}

void Player::setState(PlayerState _state)
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
