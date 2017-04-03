#include <SFML/Graphics/Color.hpp>

#include "JHelper.h"
#include "Vector2i.h"
#include "Constants.h"

int JHelper::calculateIndex(const unsigned int _x, const unsigned int _y, const unsigned int _size_x)
{
    return (_y * _size_x) + _x;
}



int JHelper::calculateIndex(const Vector2i& _pos, const unsigned int _size_x)
{
    return calculateIndex(_pos.x, _pos.y, _size_x);
}



CellValue JHelper::idToCellValue(const unsigned int _id)
{
    if (_id >= MAX_PLAYERS)
    {
        return CellValue::NONE;
    }

    return static_cast<CellValue>(static_cast<int>(CellValue::CYAN) + _id);
}



sf::Color JHelper::evaluateSFColor(const CellValue _value)
{
    switch (_value)
    {
        case CellValue::CYAN: return sf::Color::Cyan;
        case CellValue::GREEN: return sf::Color::Green;
        case CellValue::MAGENTA: return sf::Color::Magenta;
        case CellValue::YELLOW: return sf::Color::Yellow;

        default: return sf::Color::Transparent;
    }
}

sf::Color JHelper::evaluateSFColorFromCellValueID(const unsigned _id)
{
    return evaluateSFColor(idToCellValue(_id));
}
