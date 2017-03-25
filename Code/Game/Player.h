#pragma once
#include "MoveDirection.h"
#include "Colour.h"
#include "Vector2i.h"

class Player
{
public:
    Player();
    ~Player() = default;

    int getID() const;
    void setID(int _id);

    Colour getColour() const;
    void setColour(Colour _colour);

    MoveDirection getDirection() const;
    void setDirection(MoveDirection _direction);

    Vector2i getPosition() const;
    void setPosition(const Vector2i& _pos);
    
    float getMoveSpeed() const;
    void setMoveSpeed(float _speed);

    double getMoveTimer() const;
    void resetMoveTimer();
    void modifyMoveTimer(double _dt);
    
    bool isAlive() const;
    void setAlive(bool _value);

    bool isBoosting() const;
    void setBoosting(bool _value);

private:
    int id;
    Colour colour;
    MoveDirection direction;
    Vector2i pos;
    float move_speed;
    double move_timer;
    bool alive;
    bool boosting;

};
