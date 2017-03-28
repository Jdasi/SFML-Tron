#pragma once
#include <array>

#include "Constants.h"
#include "Cell.h"

struct Vector2i;

class Grid
{
public:
    Grid() = default;
    ~Grid() = default;

    Cell getCell(Vector2i _pos) const;
    void setCell(Vector2i _pos, Cell _cell);

    const std::array<Cell, GRID_AREA>& getCells() const;
    void setCells(std::array<Cell, GRID_AREA>& _cells);

private:
    int calculateCellIndex(Vector2i _pos) const;

    std::array<Cell, GRID_AREA> cells;

};
