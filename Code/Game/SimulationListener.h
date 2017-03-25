#pragma once
#include <vector>

class Player;
enum CellValue;
enum MoveDirection;
struct Cell;

class SimulationListener
{
public:
    SimulationListener() = default;
    virtual ~SimulationListener() = default;

    virtual void updateCell(const Player& _player, CellValue _value) = 0;
    virtual void updateAllCells(std::vector<Cell> _cells) = 0;

};
