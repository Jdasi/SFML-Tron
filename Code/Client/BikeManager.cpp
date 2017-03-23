#include "BikeManager.h"
#include "GameGrid.h"
#include "ClientData.h"
#include <Game/Constants.h>
#include <Game/RandomEngine.h>

BikeManager::BikeManager(GameGrid& _game_grid)
    : game_grid(_game_grid)
    , colors({
        sf::Color::Cyan,
        sf::Color::Green,
        sf::Color::Magenta,
        sf::Color::Yellow})
{
    bikes.reserve(10);
}

void BikeManager::tick(ClientData* _client_data)
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

void BikeManager::addBike()
{
    if (bikes.size() >= MAX_PLAYERS)
    {
        return;
    }

    Bike bike;

    bike.setID(bikes.size()); // Set unique bike ID starting from 0.
    bike.setTrailColor(colors[bikes.size()]);

    bike.setGridPosition({ 0, 0 }); // Need to change this to support multiple bikes...
    game_grid.setTileColor(bike.getGridPosition(), bike.getTrailColor());

    bikes.push_back(bike);
}

void BikeManager::changeBikeDirection(unsigned int _bike_id, MoveDirection _dir)
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

bool BikeManager::directionChangeValid(Bike& _bike, MoveDirection _dir) const
{
    if (_dir == MoveDirection::UP && _bike.getMoveDirection() == MoveDirection::DOWN ||
        _dir == MoveDirection::DOWN && _bike.getMoveDirection() == MoveDirection::UP ||
        _dir == MoveDirection::LEFT && _bike.getMoveDirection() == MoveDirection::RIGHT ||
        _dir == MoveDirection::RIGHT && _bike.getMoveDirection() == MoveDirection::LEFT ||
        _dir == _bike.getMoveDirection())
    {
        return false;
    }

    return true;
}

void BikeManager::moveBike(Bike& _bike) const
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

sf::Vector2<int> BikeManager::generatePositionAdjustment(MoveDirection _dir, 
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

bool BikeManager::adjustmentWithinBounds(sf::Vector2<int> _adjustment) const
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
bool BikeManager::adjustmentCollisionCheck(sf::Vector2<int> _adjustment) const
{
    if (game_grid.getTileColor(_adjustment) == sf::Color::Transparent)
    {
        return false;
    }

    return true;
}
