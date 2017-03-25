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

private:
    int calculateCellIndex(Vector2i _pos) const;

    std::vector<Cell> cells;

};
