#include "GridController.h"
#include "GameGrid.h"
#include "ClientData.h"
#include <Game/Constants.h>

GridController::GridController(GameGrid& _game_grid)
    : game_grid(_game_grid)
{
    bikes.reserve(10);
}

void GridController::tick(ClientData* _client_data)
{
    for (auto& bike : bikes)
    {
        if (!bike.isAlive())
        {
            continue;
        }

        bike.modifyMoveTimer(_client_data->delta_time);

        if (bike.getMoveTimer() > bike.getMoveSpeed())
        {
            bike.resetMoveTimer();

            moveBike(bike);
        }
    }
}

void GridController::addBike()
{
    Bike bike;

    bike.setID(bikes.size()); // Set unique bike ID starting from 0.
    bike.setGridPosition({ 0, 0 }); // Need to randomise this for more bikes...
    bike.setTrailColor(sf::Color::Red); // Need to randomise this for more bikes...

    bikes.push_back(bike);
}

void GridController::changeBikeDirection(unsigned int _bike_id, MoveDirection _dir)
{
    if (_bike_id > bikes.size())
    {
        return;
    }

    Bike& bike = bikes[_bike_id];
    if (directionChangeValid(bike, _dir))
    {
        bike.setMoveDirection(_dir);
    }
}

bool GridController::directionChangeValid(Bike& _bike, MoveDirection _dir) const
{
    if (_dir == MoveDirection::UP && _bike.getMoveDirection() == MoveDirection::DOWN ||
        _dir == MoveDirection::DOWN && _bike.getMoveDirection() == MoveDirection::UP ||
        _dir == MoveDirection::LEFT && _bike.getMoveDirection() == MoveDirection::RIGHT ||
        _dir == MoveDirection::RIGHT && _bike.getMoveDirection() == MoveDirection::LEFT)
    {
        return false;
    }

    return true;
}

void GridController::moveBike(Bike& _bike) const
{
    MoveDirection dir = _bike.getMoveDirection();
    sf::Vector2<int> pos = _bike.getGridPosition();

    sf::Vector2<int> adjustment = pos + generatePositionAdjustment(dir, pos);

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
        _bike.setGridPosition(adjustment);
        game_grid.setTileColor(adjustment, _bike.getTrailColor());
    }
}

sf::Vector2<int> GridController::generatePositionAdjustment(MoveDirection _dir, 
    sf::Vector2<int> _current_position) const
{
    switch(_dir)
    {
        case MoveDirection::UP:     return {  0 , -1 };
        case MoveDirection::DOWN:   return {  0 ,  1 };
        case MoveDirection::LEFT:   return { -1 ,  0 };
        case MoveDirection::RIGHT:  return {  1 ,  0 };

        default: return { 0, 0 };
    }
}

bool GridController::adjustmentWithinBounds(sf::Vector2<int> _adjustment) const
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

// Returns true if the adjustment points to a coloured tile, otherwise returns false.
bool GridController::adjustmentCollisionCheck(sf::Vector2<int> _adjustment) const
{
    if (game_grid.getTileColor(_adjustment) == sf::Color::Transparent)
    {
        return false;
    }

    return true;
}
