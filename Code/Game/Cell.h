#pragma once
#include "CellValue.h"
#include "CellColour.h"

struct Cell
{
    Cell()
        : value(CellValue::NONE)
        , colour(CellColour::CYAN)
    {
    }

    Cell(CellValue _value, CellColour _colour)
        : value(_value)
        , colour(_colour)
    {
    }

    CellValue value;
    CellColour colour;
};
