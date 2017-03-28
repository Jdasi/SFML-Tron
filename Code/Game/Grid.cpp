#include "Grid.h"
#include "Constants.h"
#include "Vector2i.h"

Cell Grid::getCell(Vector2i _pos) const
{
    return cells[calculateCellIndex(_pos)];
}

void Grid::setCell(Vector2i _pos, Cell _cell)
{
    cells[calculateCellIndex(_pos)] = _cell;
}

const std::array<Cell, GRID_AREA>& Grid::getCells() const
{
    return cells;
}

void Grid::setCells(std::array<Cell, GRID_AREA>& _cells)
{
    cells = _cells;
}

int Grid::calculateCellIndex(Vector2i _pos) const
{
    return (_pos.y * GRID_SIZE_X) + _pos.x;
}
