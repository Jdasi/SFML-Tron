#include <SFML/Network.hpp>

#include "Simulation.h"
#include "Grid.h"
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

void Simulation::overwrite(const Simulation& _simulation)
{
    grid = _simulation.grid;
    bikes = _simulation.bikes;

    for (auto& listener : listeners)
    {
        listener->updateAllCells(grid.getCells());
    }
}

void Simulation::reset()
{
    auto cells = grid.getCells();
    for (auto& cell : cells)
    {
        cell.value = CellValue::NONE;
    }

    grid.setCells(cells);
    bikes.clear();
    colours_assigned = 0;
}

const Grid& Simulation::getGrid() const
{
    return grid;
}

const std::vector<Bike>& Simulation::getBikes() const
{
    return bikes;
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

sf::Packet& operator<<(sf::Packet& _packet, const Simulation& _simulation)
{
    auto& cells = _simulation.getGrid().getCells();
    _packet << static_cast<sf::Uint32>(cells.size());
    for (auto& cell : cells)
    {
        _packet << static_cast<sf::Uint8>(cell.value) 
                << static_cast<sf::Uint8>(cell.colour);
    }

    auto& bikes = _simulation.getBikes();
    _packet << static_cast<sf::Uint8>(bikes.size());
    for (auto& bike : bikes)
    {
        _packet << static_cast<sf::Uint8>(bike.getID())
                << static_cast<sf::Uint8>(bike.getColour())
                << static_cast<sf::Uint8>(bike.getDirection())
                << static_cast<sf::Uint32>(bike.getPosition().x)
                << static_cast<sf::Uint32>(bike.getPosition().y)
                << bike.isAlive()
                << bike.isBoosting();
    }

    return _packet;
}

sf::Packet& operator>>(sf::Packet& _packet, Simulation& _simulation)
{
    sf::Uint32 num_cells;
    _packet >> num_cells;

    std::vector<Cell> cells(num_cells);
    for (sf::Uint32 i = 0; i < num_cells; ++i)
    {
        sf::Uint8 cell_value;
        sf::Uint8 cell_colour;

        _packet >> cell_value >> cell_colour;
        cells[i].value = static_cast<CellValue>(cell_value);
        cells[i].colour = static_cast<CellColour>(cell_colour);
    }
    _simulation.grid.setCells(cells);

    sf::Uint8 num_bikes;
    _packet >> num_bikes;

    std::vector<Bike> bikes(num_bikes);
    for (sf::Uint8 i = 0; i < num_bikes; ++i)
    {
        sf::Uint8   bike_id;
        sf::Uint8   bike_col;
        sf::Uint8   bike_dir;
        Vector2i    bike_pos;
        bool        bike_alive;
        bool        bike_boosting;

        _packet >> bike_id >> bike_col >> bike_dir >> bike_pos.x >> bike_pos.y 
                >> bike_alive >> bike_boosting;

        Bike& bike = bikes[i];
        bike.setID(bike_id);
        bike.setDirection(static_cast<MoveDirection>(bike_dir));
        bike.setPosition(bike_pos);
        bike.setAlive(bike_alive);
        bike.setBoosting(bike_boosting);
    }
    _simulation.bikes = bikes;

    return _packet;
}
