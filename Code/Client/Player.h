#pragma once
#include <Game/PlayerState.h>

class Player
{
public:
    Player(int _id, PlayerState _state = PlayerState::NOTREADY);
    ~Player() = default;

    int getID() const;
    void setID(int _id);

    PlayerState getState() const;
    void setState(PlayerState _state);

private:
    int id;
    PlayerState state;

};
