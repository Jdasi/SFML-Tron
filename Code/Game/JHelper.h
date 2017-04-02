#pragma once

struct Vector2i;

namespace JHelper
{
    int calculateIndex(const unsigned int _x, const unsigned int _y, const unsigned int _size_x);
    int calculateIndex(const Vector2i& _pos, const unsigned int _size_x);
}
