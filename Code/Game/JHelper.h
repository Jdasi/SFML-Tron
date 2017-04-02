#pragma once

namespace sf
{
    class Color;
}

#include "CellValue.h"
#include "Vector2i.h"

namespace JHelper
{
    int calculateIndex(const unsigned int _x, const unsigned int _y, const unsigned int _size_x);
    int calculateIndex(const Vector2i& _pos, const unsigned int _size_x);

    CellValue idToCellValue(const unsigned int _id);
    sf::Color evaluateSFColor(const CellValue _value);

}
