#pragma once
#include <vector>

class Bike;
enum CellValue;
enum MoveDirection;
struct Cell;

class SimulationListener
{
public:
    SimulationListener() = default;
    virtual ~SimulationListener() = default;

    virtual void updateCell(const Bike& _bike, CellValue _value) = 0;
    virtual void updateAllCells(std::vector<Cell> _cells) = 0;

};
