#pragma once
#include <vector>
#include "BikeState.h"

namespace sf
{
    class Packet;
}

#include "MoveDirection.h"
#include "Vector2i.h"
#include "CellValue.h"

class Bike
{
public:
    Bike();
    ~Bike() = default;

    void tick(double _dt);

    unsigned int getID() const;
    void setID(const unsigned int _id);

    CellValue idToCellValue() const;

    BikeState getState() const;
    void overwriteState(const BikeState& _state);

    MoveDirection getDirection() const;
    void setDirection(const MoveDirection _direction);

    Vector2i getPosition() const;
    void setPosition(const Vector2i& _pos);

    const std::vector<Vector2i>& getLine() const;
    void setLine(const std::vector<Vector2i>& _line);
    
    float getMoveSpeed() const;
    void setMoveSpeed(const float _speed);

    double getMoveTimer() const;
    void setMoveTimer(const double _value);
    void resetMoveTimer();
    void modifyMoveTimer(const double _dt);
    
    bool isAlive() const;
    void setAlive(const bool _value);

    bool isBoosting() const;
    bool boost();

private:
    BikeState state;

};
