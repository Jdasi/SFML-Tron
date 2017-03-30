#include <SFML/Network.hpp>

#include "Simulation.h"
#include "Grid.h"
#include "Bike.h"
#include "Constants.h"

Simulation::Simulation()
{
    setBikeIDs();
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

void Simulation::addBike(unsigned int _id)
{
    if (_id >= MAX_PLAYERS)
    {
        return;
    }

    Bike& bike = bikes[_id];
    bike.setColour(static_cast<CellColour>(_id));

    configureBikeSide(bike);
    grid.setCell(bike.getPosition(), { CellValue::HEAD, bike.getColour() });

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
    auto& old_positions = bike.getLine();

    // Erase old line.
    for (auto& pos : old_positions)
    {
        grid.setCell(pos, { CellValue::NONE, CellColour::CYAN });
    }

    // Overwrite Bike.
    bike = _bike;
    auto& new_positions = bike.getLine();

    // Write new line.
    for (auto& pos : old_positions)
    {
        grid.setCell(pos, { CellValue::TRAIL, bike.getColour() });
    }

    grid.setCell(bike.getPosition(), { CellValue::HEAD, bike.getColour() });

    // Inform listeners of update.
    for (auto& listener : listeners)
    {
        listener->overwriteCellRange(new_positions, CellValue::TRAIL, _bike.getColour());
        listener->overwriteCell(_bike.getPosition(), CellValue::HEAD);
    }
}

void Simulation::reset()
{
    grid.reset();
    setBikeIDs();
}

const Grid& Simulation::getGrid() const
{
    return grid;
}

Bike& Simulation::getBike(unsigned int _bike_id)
{
    return bikes[_bike_id];
}

const std::array<Bike, MAX_PLAYERS>& Simulation::getBikes() const
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
    grid.setCell(_bike.getPosition(), { CellValue::TRAIL, _bike.getColour() } );

    for (auto& listener : listeners)
    {
        listener->overwriteCell(_bike.getPosition(), CellValue::TRAIL,_bike.getColour());
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
            listener->overwriteCell(_bike.getPosition(), CellValue::HEAD, _bike.getColour());
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

void Simulation::setBikeIDs()
{
    int id = 0;
    for (auto& bike : bikes)
    {
        bike = Bike();
        bike.setID(id++);
    }
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
        _packet << bike;
    }

    return _packet;
}

sf::Packet& operator>>(sf::Packet& _packet, Simulation& _simulation)
{
    sf::Uint32 num_cells;
    _packet >> num_cells;

    std::array<Cell, GRID_AREA> cells;
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

    std::array<Bike, MAX_PLAYERS> bikes;
    for (sf::Uint8 i = 0; i < num_bikes; ++i)
    {
        _packet >> bikes[i];
    }
    _simulation.bikes = bikes;

    return _packet;
}
