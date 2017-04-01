#include <SFML/Network.hpp>

#include "Simulation.h"
#include "Grid.h"
#include "Bike.h"
#include "Constants.h"
#include <iostream>

Simulation::Simulation()
{
    resetBikes();
}

void Simulation::tick(double _dt)
{
    for (auto& bike : bikes)
    {
        if (!bike.isAlive())
        {
            continue;
        }

        bike.tick(_dt);
        bike.modifyMoveTimer(_dt);

        if (bike.getMoveTimer() > bike.getMoveSpeed())
        {
            bike.resetMoveTimer();

            moveBike(bike);
        }
    }
}

void Simulation::addBike(unsigned int _id)
{
    if (_id >= MAX_PLAYERS)
    {
        return;
    }

    Bike& bike = bikes[_id];

    configureBikeSide(bike);
    grid.setCellValue(bike.getPosition(), bike.idToCellValue());

    for (auto& listener : listeners)
    {
        listener->updateBikePosition(bike.getPosition(), bike.getID());
        //listener->addPlayerMarker(bike.getID(), bike.idToCellValue());
    }

    bike.setAlive(true);
}

void Simulation::overwrite(const Simulation& _simulation)
{
    grid = _simulation.grid;
    bikes = _simulation.bikes;

    for (auto& listener : listeners)
    {
        listener->overwriteAllCells(grid.getCells());
    }
}

void Simulation::overwriteBike(const Bike& _bike)
{
    Bike& bike = bikes[_bike.getID()];

    // Erase old line.
    grid.clearCellRange(bike.getLine());

    for (auto& listener : listeners)
    {
        listener->clearCellRange(bike.getLine());
    }

    // Overwrite Bike.
    bike = _bike;

    // Write new line.
    grid.overwriteCellRange(bike.getLine(), bike.idToCellValue());

    for (auto& listener : listeners)
    {
        listener->overwriteCellRange(bike.getLine(), bike.idToCellValue());
    }

    handleBikeDeath(bike);
}

void Simulation::overwriteBikes(const std::array<Bike, MAX_PLAYERS>& _bikes)
{
    for (auto& bike : _bikes)
    {
        if (!bike.isAlive())
        {
            continue;
        }

        overwriteBike(bike);
    }
}

void Simulation::reset()
{
    grid.reset();
    resetBikes();

    for (auto& listener : listeners)
    {
        listener->clearAllCells();
    }
}

const Grid& Simulation::getGrid() const
{
    return grid;
}

Bike& Simulation::getBike(unsigned int _bike_id)
{
    return bikes[_bike_id];
}

bool Simulation::allBikesDead() const
{
    for (auto& bike : bikes)
    {
        if (bike.isAlive())
        {
            return false;
        }
    }

    return true;
}

std::array<Bike, MAX_PLAYERS>& Simulation::getBikes()
{
    return bikes;
}

void Simulation::changeBikeDirection(unsigned int _bike_id, const MoveDirection _dir)
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

void Simulation::configureBikeSide(Bike& _bike) const
{
    int x_pos_left = 0;
    int x_pos_right = GRID_SIZE_X - 1;

    int y_pos_top = static_cast<int>(GRID_SIZE_Y * 0.25f);
    int y_pos_bottom = static_cast<int>(GRID_SIZE_Y * 0.75f);

    switch (_bike.getID())
    {
        case 0:
        {
            _bike.setPosition({ x_pos_left, y_pos_top });
        } break;

        case 1:
        {
            _bike.setPosition({ x_pos_right, y_pos_bottom });
            _bike.setDirection(MoveDirection::LEFT);
        } break;

        case 2:
        {
            _bike.setPosition({ x_pos_left, y_pos_bottom });
        } break;

        case 3:
        {
            _bike.setPosition({ x_pos_right, y_pos_top });
            _bike.setDirection(MoveDirection::LEFT);
        } break;

        default: {}
    }
}

void Simulation::moveBike(Bike& _bike)
{
    grid.setCellValue(_bike.getPosition(), _bike.idToCellValue());
    
    for (auto& listener : listeners)
    {
        listener->overwriteCell(_bike.getPosition(), _bike.idToCellValue());
    }

    MoveDirection dir = _bike.getDirection();
    Vector2i pos = _bike.getPosition();

    Vector2i adjustment = generatePositionAdjustment(dir, pos);

    if (!adjustmentWithinBounds(adjustment))
    {
        // Bike hit the edge of the grid.
        _bike.setAlive(false);
    }
    else if (adjustmentCollisionCheck(adjustment))
    {
        // Bike hit a line.
        _bike.setAlive(false);
    }
    else
    {
        // Path is clear.
        _bike.setPosition(adjustment);
        grid.setCellValue(adjustment, _bike.idToCellValue());

        for (auto& listener : listeners)
        {
            listener->updateBikePosition(_bike.getPosition(), _bike.getID());
        }
    }

    handleBikeDeath(_bike);
}

Vector2i Simulation::generatePositionAdjustment(const MoveDirection _dir, 
    const Vector2i& _current_pos) const
{
    Vector2i adjustment;

    switch (_dir)
    {
        case MoveDirection::UP:     adjustment = {  0, -1 }; break;
        case MoveDirection::DOWN:   adjustment = {  0,  1 }; break;
        case MoveDirection::LEFT:   adjustment = { -1,  0 }; break;
        case MoveDirection::RIGHT:  adjustment = {  1,  0 }; break;

        default: adjustment = { 0, 0 };
    }

    return _current_pos + adjustment;
}

void Simulation::handleBikeDeath(const Bike& _bike)
{
    if (!_bike.isAlive())
    {
        for (auto& listener : listeners)
        {
            listener->removePlayerMarker(_bike.getID());
        }
    }
}

// Returns true if adjustment points to a grid space, otherwise returns false.
bool Simulation::adjustmentWithinBounds(const Vector2i& _adjustment) const
{
    if (_adjustment.x < GRID_SIZE_X &&
        _adjustment.x >= 0 &&
        _adjustment.y < GRID_SIZE_Y &&
        _adjustment.y >= 0)
    {
        return true;
    }

    return false;
}

// Returns true if there is a collision, otherwise returns false.
bool Simulation::adjustmentCollisionCheck(const Vector2i& _adjustment) const
{
    if (grid.getCellValue(_adjustment) != CellValue::NONE)
    {
        return true;
    }

    return false;
}

// Returns true if direction change is valid, otherwise returns false.
bool Simulation::directionChangeValid(const Bike& _bike, const MoveDirection _new_dir) const
{
    MoveDirection bike_dir = _bike.getDirection();

    if (_bike.isAlive() &&
        (bike_dir != _new_dir) &&
        (bike_dir == MoveDirection::UP && _new_dir != MoveDirection::DOWN) ||
        (bike_dir == MoveDirection::DOWN && _new_dir != MoveDirection::UP) ||
        (bike_dir == MoveDirection::LEFT && _new_dir != MoveDirection::RIGHT) ||
        (bike_dir == MoveDirection::RIGHT && _new_dir != MoveDirection::LEFT))
    {
        return true;
    }

    return false;
}

// Goes through the array of bikes and overwrites them with new copies with correct ids.
void Simulation::resetBikes()
{
    int id = 0;
    for (auto& bike : bikes)
    {
        bike = Bike();
        bike.setID(id++);
    }

    for (auto& listener : listeners)
    {
        listener->removeAllPlayerMarkers();
    }
}

sf::Packet& operator<<(sf::Packet& _packet, Simulation& _simulation)
{
    auto& cells = _simulation.getGrid().getCells();
    for (auto& cell_value : cells)
    {
        _packet << static_cast<sf::Uint8>(cell_value);
    }

    auto& bikes = _simulation.getBikes();
    for (auto& bike : bikes)
    {
        _packet << bike;
    }

    return _packet;
}

sf::Packet& operator>>(sf::Packet& _packet, Simulation& _simulation)
{
    std::array<CellValue, GRID_AREA> cells;
    for (unsigned int i = 0; i < GRID_AREA; ++i)
    {
        sf::Uint8 cell_value;

        _packet >> cell_value;
        cells[i] = static_cast<CellValue>(cell_value);
    }
    _simulation.grid.overwriteAllCells(cells);

    std::array<Bike, MAX_PLAYERS> bikes;
    for (sf::Uint8 i = 0; i < MAX_PLAYERS; ++i)
    {
        _packet >> bikes[i];
    }
    _simulation.bikes = bikes;

    return _packet;
}
