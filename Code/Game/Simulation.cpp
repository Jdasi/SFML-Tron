#include "Simulation.h"
#include "Grid.h"
#include "Bike.h"
#include "Constants.h"

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
    }

    bike.setAlive(true);
}

void Simulation::overwrite(const SimulationState& _simulation_state)
{
    grid.overwriteAllCells(_simulation_state.cells);

    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        bikes[i].overwriteState(_simulation_state.bikes[i]);

        for (auto& listener : listeners)
        {
            listener->updatePlayerMarker(_simulation_state.bikes[i]);
        }
    }

    for (auto& listener : listeners)
    {
        listener->overwriteAllCells(grid.getCells());
    }
}

void Simulation::overwriteBike(const BikeState& _bike_state)
{
    Bike& bike = bikes[_bike_state.id];

    // Erase old line.
    grid.clearCellRange(bike.getLine());

    for (auto& listener : listeners)
    {
        listener->clearCellRange(bike.getLine());
    }

    // Overwrite Bike.
    bike.overwriteState(_bike_state);

    // Write new line.
    grid.overwriteCellRange(bike.getLine(), bike.idToCellValue());

    for (auto& listener : listeners)
    {
        listener->overwriteCellRange(bike.getLine(), bike.idToCellValue());
        listener->updatePlayerMarker(_bike_state);
    }
}

void Simulation::overwriteBikes(const std::array<BikeState, MAX_PLAYERS>& _bike_states)
{
    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        overwriteBike(_bike_states[i]);
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

SimulationState Simulation::getState() const
{
    SimulationState state;

    state.cells = grid.getCells();
    
    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        state.bikes[i] = bikes[i].getState();
    }

    return state;
}

void Simulation::overwriteState(const SimulationState& _state)
{
    grid.overwriteAllCells(_state.cells);

    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        bikes[i].overwriteState(bikes[i].getState());
    }
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

std::array<BikeState, MAX_PLAYERS> Simulation::getBikes()
{
    std::array<BikeState, MAX_PLAYERS> bike_states;

    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        bike_states[i] = bikes[i].getState();
    }

    return bike_states;
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
