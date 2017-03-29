#pragma once
#include <vector>
#include <array>

#include <Game/Constants.h>
#include <Game/Cell.h>
#include <Game/MoveDirection.h>

class Bike;
struct Vector2i;

class SimulationListener
{
public:
    SimulationListener() = default;
    virtual ~SimulationListener() = default;

    virtual void overwriteCell(const Vector2i& _pos,
        const CellValue _value = CellValue::NONE,
        const CellColour _colour = CellColour::CYAN) = 0;

    virtual void overwriteCellRange(const std::vector<Vector2i>& _positions,
        const CellValue _value = CellValue::NONE,
        const CellColour _colour = CellColour::CYAN) = 0;

    virtual void overwriteAllCells(const std::array<Cell, GRID_AREA>& _cells) = 0;

};
