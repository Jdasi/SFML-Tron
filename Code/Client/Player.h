#pragma once
#include <Game/PlayerState.h>

class Player
{
public:
    Player(const unsigned int _id, PlayerState _state = PlayerState::NOTREADY);
    ~Player() = default;

    unsigned getID() const;
    void setID(const unsigned int _id);

    PlayerState getState() const;
    void setState(PlayerState _state);

private:
    unsigned int id;
    PlayerState state;

};
