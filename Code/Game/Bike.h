#pragma once
#include <vector>

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

    int getID() const;
    void setID(int _id);

    CellValue getCellValue() const;

    MoveDirection getDirection() const;
    void setDirection(MoveDirection _direction);

    Vector2i getPosition() const;
    void setPosition(const Vector2i& _pos);

    const std::vector<Vector2i>& getLine() const;
    void setLine(const std::vector<Vector2i>& _line);
    
    float getMoveSpeed() const;
    void setMoveSpeed(float _speed);

    double getMoveTimer() const;
    void setMoveTimer(double _value);
    void resetMoveTimer();
    void modifyMoveTimer(double _dt);
    
    bool isAlive() const;
    void setAlive(bool _value);

    bool isBoosting() const;
    void setBoosting(bool _value);

    friend sf::Packet& operator<<(sf::Packet& _packet, const Bike& _bike);
    friend sf::Packet& operator>>(sf::Packet& _packet, Bike& _bike);

private:
    int id;
    MoveDirection direction;
    Vector2i pos;
    std::vector<Vector2i> line;
    float move_speed;
    double move_timer;
    bool alive;
    bool boosting;

};
