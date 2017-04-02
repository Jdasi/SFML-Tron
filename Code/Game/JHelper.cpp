#include "JHelper.h"
#include "Vector2i.h"

int JHelper::calculateIndex(const unsigned int _x, const unsigned int _y, const unsigned int _size_x)
{
    return (_y * _size_x) + _x;
}

int JHelper::calculateIndex(const Vector2i& _pos, const unsigned int _size_x)
{
    return calculateIndex(_pos.x, _pos.y, _size_x);
}