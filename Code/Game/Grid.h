#pragma once
#include <vector>

#include "Cell.h"

struct Vector2i;

class Grid
{
public:
    Grid();
    ~Grid() = default;

    Cell getCell(Vector2i _pos) const;
    void setCell(Vector2i _pos, Cell _cell);

    const std::vector<Cell>& getCells() const;
    void setCells(std::vector<Cell>& _cells);

private:
    int calculateCellIndex(Vector2i _pos) const;

    std::vector<Cell> cells;

};
