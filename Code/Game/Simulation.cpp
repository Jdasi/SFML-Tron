#include "Simulation.h"
#include "Bike.h"
#include "Constants.h"

Simulation::Simulation()
    : colours_assigned(0)
{
    bikes.reserve(MAX_PLAYERS);
}

void Simulation::tick(double _dt)
{
    for (auto& bike : bikes)
    {
        if (!bike.isAlive())
        {
            continue;
        }

        bike.modifyMoveTimer(_dt);

        if (bike.getMoveTimer() > bike.getMoveSpeed())
        {
            bike.resetMoveTimer();

            moveBike(bike);
        }
    }
}

void Simulation::addBike()
{
    if (bikes.size() >= MAX_PLAYERS)
    {
        return;
    }

    Bike bike;

    bike.setID(bikes.size());
    bike.setColour(static_cast<CellColour>(colours_assigned++));

    bike.setPosition({ 0, bike.getID() * 20 }); // Need to change this to properly support multiple bikes...
    grid.setCell(bike.getPosition(), { CellValue::HEAD, bike.getColour() });

    bikes.push_back(bike);
}

void Simulation::changeBikeDirection(unsigned int _bike_id, MoveDirection _dir)
{
    if (_bike_id > bikes.size() || bikes.empty())
    {
        return;
    }

    Bike& bike = bikes[_bike_id];
    if (directionChangeValid(bike, _dir))
    {
        bike.setDirection(_dir);
    }
}

void Simulation::moveBike(Bike& _bike)
{
    grid.setCell(_bike.getPosition(), { CellValue::TRAIL, _bike.getColour() } );

    for (auto& listener : listeners)
    {
        listener->updateCell(_bike, CellValue::TRAIL);
    }

    MoveDirection dir = _bike.getDirection();
    Vector2i pos = _bike.getPosition();

    Vector2i adjustment = pos + generatePositionAdjustment(dir, pos);

    if (!adjustmentWithinBounds(adjustment))
    {
        // Bike hit the edge of the grid.
        _bike.setAlive(false);
    }
    else if (adjustmentCollisionCheck(adjustment))
    {
        // Bike hit a trail.
        _bike.setAlive(false);
    }
    else
    {
        // Path is clear.
        _bike.setPosition(adjustment);
        grid.setCell(adjustment, { CellValue::HEAD, _bike.getColour() });

        for (auto& listener : listeners)
        {
            listener->updateCell(_bike, CellValue::HEAD);
        }
    }
}

Vector2i Simulation::generatePositionAdjustment(MoveDirection _dir, Vector2i _current_pos) const
{
    switch (_dir)
    {
        case MoveDirection::UP:     return {  0, -1 };
        case MoveDirection::DOWN:   return {  0,  1 };
        case MoveDirection::LEFT:   return { -1,  0 };
        case MoveDirection::RIGHT:  return {  1,  0 };

        default: return { 0, 0 };
    }
}

bool Simulation::adjustmentWithinBounds(Vector2i _adjustment) const
{
    if (_adjustment.x >= GRID_SIZE_X ||
        _adjustment.x < 0 ||
        _adjustment.y >= GRID_SIZE_Y ||
        _adjustment.y < 0)
    {
        return false;
    }

    return true;
}

bool Simulation::adjustmentCollisionCheck(Vector2i _adjustment) const
{
    if (grid.getCell(_adjustment).value == CellValue::NONE)
    {
        return false;
    }

    return true;
}

bool Simulation::directionChangeValid(Bike& _bike, MoveDirection _dir)
{
    if (_dir == MoveDirection::UP && _bike.getDirection() == MoveDirection::DOWN ||
        _dir == MoveDirection::DOWN && _bike.getDirection() == MoveDirection::UP ||
        _dir == MoveDirection::LEFT && _bike.getDirection() == MoveDirection::RIGHT ||
        _dir == MoveDirection::RIGHT && _bike.getDirection() == MoveDirection::LEFT ||
        _dir == _bike.getDirection())
    {
        return false;
    }

    return true;
}
