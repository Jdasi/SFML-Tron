#include "Grid.h"
#include "Constants.h"
#include "Vector2i.h"

Grid::Grid()
{
    cells.assign(GRID_SIZE_X * GRID_SIZE_Y, CellValue::NONE);
}

int Grid::getCellValue(Vector2i _pos) const
{
    return cells[calculateCellIndex(_pos)];
}

void Grid::setCellValue(Vector2i _pos, CellValue _value)
{
    cells[calculateCellIndex(_pos)] = _value;
}

int Grid::calculateCellIndex(Vector2i _pos) const
{
    return (_pos.y * GRID_SIZE_X) + _pos.x;
}
