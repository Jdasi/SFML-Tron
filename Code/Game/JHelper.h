#pragma once
#include <algorithm>

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

    template <typename T, typename U>
    auto findInVectorPair(T& _vector, U& _key)
    {
        auto entry = std::find_if(_vector.begin(), _vector.end(),
            [_key](const auto& _elem)
        {
            return _elem.first == _key;
        });

        return entry;
    }

    template <typename T>
    void sortVectorPair(T& _vector)
    {
        std::sort(_vector.begin(), _vector.end(),
            [](const auto& _lhs, const auto& _rhs)
        {
            return _lhs.first < _rhs.first;
        });
    }
}
