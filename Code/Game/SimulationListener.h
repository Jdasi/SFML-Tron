#pragma once

class Player;
enum CellValue;
enum MoveDirection;

class SimulationListener
{
public:
    SimulationListener() = default;
    virtual ~SimulationListener() = default;

    virtual void updateCell(const Player& _player, CellValue _value) = 0;

};
