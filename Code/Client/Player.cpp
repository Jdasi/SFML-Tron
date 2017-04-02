#include "Player.h"

Player::Player(const unsigned int _id, PlayerState _state)
    : id(_id)
    , state(_state)
{
}

unsigned int Player::getID() const
{
    return id;
}

void Player::setID(const unsigned int _id)
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
