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

    unsigned int getID() const;
    void setID(const unsigned int _id);

    CellValue idToCellValue() const;

    MoveDirection getDirection() const;
    void setDirection(const MoveDirection _direction);

    Vector2i getPosition() const;
    void setPosition(const Vector2i& _pos);

    const std::vector<Vector2i>& getLine() const;
    void setLine(const std::vector<Vector2i>& _line);

    unsigned int getSyncIndex() const;
    void setSyncIndex(const unsigned int _value);
    
    float getMoveSpeed() const;
    void setMoveSpeed(const float _speed);

    double getMoveTimer() const;
    void setMoveTimer(const double _value);
    void resetMoveTimer();
    void modifyMoveTimer(const double _dt);
    
    bool isAlive() const;
    void setAlive(const bool _value);

    bool isBoosting() const;
    void setBoosting(const bool _value);

    friend sf::Packet& operator<<(sf::Packet& _packet, Bike& _bike);
    friend sf::Packet& operator>>(sf::Packet& _packet, Bike& _bike);

private:
    unsigned int id;
    MoveDirection direction;
    Vector2i pos;
    std::vector<Vector2i> line;
    unsigned int line_sync_index;
    float move_speed;
    double move_timer;
    bool alive;
    bool boosting;

};
