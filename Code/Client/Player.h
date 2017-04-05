#pragma once
#include <Game/PlayerState.h>

/* Class for the client to store information about users
 * on the server (including themself).
 *
 * Used by GameManager to record up to date information about each user.
 */
class Player
{
public:
    explicit Player(const unsigned int _id, 
        const PlayerState _state = PlayerState::NOTREADY);
    ~Player() = default;

    unsigned getID() const;
    void setID(const unsigned int _id);

    PlayerState getState() const;
    void setState(const PlayerState _state);

private:
    unsigned int id;
    PlayerState state;

};
