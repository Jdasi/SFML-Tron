#pragma once
#include <SFML/Graphics.hpp>

#include <Game/MoveDirection.h>

class Bike
{
public:
    Bike();
    ~Bike() = default;

    int getID() const;
    void setID(int _id);
    
    sf::Color getTrailColor() const;
    void setTrailColor(sf::Color _color);

    sf::Vector2<int> getGridPosition() const;
    void setGridPosition(sf::Vector2<int> _grid_pos);

    MoveDirection getMoveDirection() const;
    void setMoveDirection(MoveDirection _direction);

    float getMoveSpeed() const;
    void setMoveSpeed(float _value);

    double getMoveTimer() const;
    void resetMoveTimer();
    void modifyMoveTimer(double _value);

    bool isAlive() const;
    void setAlive(bool _value);

private:
    int bike_id;
    sf::Color trail_color;
    sf::Vector2<int> grid_position;
    MoveDirection move_direction;
    float move_speed;
    double move_timer;
    bool alive;

};
