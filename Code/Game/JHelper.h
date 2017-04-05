#pragma once

namespace sf
{
    class Text;
    class Color;
    class Shape;
}

#include "CellValue.h"
#include "Vector2i.h"

/* Various functions that don't belong in one specific place and are useful
 * in many areas.
 */
namespace JHelper
{
    int calculateIndex(const unsigned int _x, const unsigned int _y, const unsigned int _size_x);
    int calculateIndex(const Vector2i& _pos, const unsigned int _size_x);

    CellValue idToCellValue(const unsigned int _id);
    sf::Color evaluateSFColor(const CellValue _value);
    sf::Color evaluateSFColorFromPlayerID(const unsigned int _id);

    template <typename T>
    void centerSFOrigin(T& _sfml_object)
    {
        auto bounds = _sfml_object.getLocalBounds();
        _sfml_object.setOrigin(bounds.left + bounds.width / 2.0f,
            bounds.top + bounds.height / 2.0f);
    }
}
