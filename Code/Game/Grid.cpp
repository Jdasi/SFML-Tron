#include "Grid.h"
#include "Constants.h"
#include "Vector2i.h"

Grid::Grid()
{
    reset();
}

void Grid::reset()
{
    for (auto& cell : cells)
    {
        cell = CellValue::NONE;
    }
}

void Grid::clearCell(const Vector2i& _pos)
{
    setCellValue(_pos, CellValue::NONE);
}

void Grid::clearCellRange(const std::vector<Vector2i>& _positions)
{
    for (auto& pos : _positions)
    {
        clearCell(pos);
    }
}

CellValue Grid::getCellValue(const Vector2i& _pos) const
{
    return cells[calculateIndex(_pos)];
}

void Grid::setCellValue(const Vector2i& _pos, const CellValue _value)
{
    cells[calculateIndex(_pos)] = _value;
}

const std::array<CellValue, GRID_AREA>& Grid::getCells() const
{
    return cells;
}

void Grid::overwriteAllCells(const std::array<CellValue, GRID_AREA>& _cells)
{
    cells = _cells;
}

void Grid::overwriteCellRange(const std::vector<Vector2i>& _positions, const CellValue _value)
{
    for (auto& pos : _positions)
    {
        setCellValue(pos, _value);
    }
}

int Grid::calculateIndex(const Vector2i& _pos) const
{
    return (_pos.y * GRID_SIZE_X) + _pos.x;
}
