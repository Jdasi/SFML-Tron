#pragma once
#include <array>
#include <vector>

#include "Constants.h"
#include "CellValue.h"

struct Vector2i;

/* Class for holding an array of values equal to the GRID_AREA
 * defined in Constants.h.
 *
 * Used by the simulation to create the gameplay.
 */
class Grid
{
public:
    Grid();
    ~Grid() = default;

    void reset();

    void clearCell(const Vector2i& _pos);
    void clearCellRange(const std::vector<Vector2i>& _positions);

    CellValue getCellValue(const Vector2i& _pos) const;
    void setCellValue(const Vector2i& _pos, const CellValue _value);

    const std::array<CellValue, GRID_AREA>& getCells() const;
    void overwriteAllCells(const std::array<CellValue, GRID_AREA>& _cells);
    void overwriteCellRange(const std::vector<Vector2i>& _positions, const CellValue _value);

private:
    std::array<CellValue, GRID_AREA> cells;

};
