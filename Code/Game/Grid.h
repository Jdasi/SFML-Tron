#pragma once
#include <vector>

#include "CellValue.h"

struct Vector2i;

class Grid
{
public:
    Grid();
    ~Grid() = default;

    int getCellValue(Vector2i _pos) const;
    void setCellValue(Vector2i _pos, CellValue _value);

private:
    int calculateCellIndex(Vector2i _pos) const;

    std::vector<CellValue> cells;

};
