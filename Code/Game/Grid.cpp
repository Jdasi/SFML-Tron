#include "Grid.h"
#include "Constants.h"
#include "Vector2i.h"

Grid::Grid()
{
    cells.assign(GRID_SIZE_X * GRID_SIZE_Y, Cell());
}

Cell Grid::getCell(Vector2i _pos) const
{
    return cells[calculateCellIndex(_pos)];
}

void Grid::setCell(Vector2i _pos, Cell _cell)
{
    cells[calculateCellIndex(_pos)] = _cell;
}

int Grid::calculateCellIndex(Vector2i _pos) const
{
    return (_pos.y * GRID_SIZE_X) + _pos.x;
}
