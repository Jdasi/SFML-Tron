#pragma once

struct Vector2i
{
    Vector2i()
        : x(0)
        , y(0)
    {
    }

    Vector2i(int _x, int _y)
        : x(_x)
        , y(_y)
    {
    }

    Vector2i& operator=(Vector2i rhs)
    {
        this->x = rhs.x;
        this->y = rhs.y;
                
        return *this;
    }

    Vector2i operator+(const Vector2i& rhs) const
    {
        Vector2i vec = *this;
        
        vec.x += rhs.x;
        vec.y += rhs.y;

        return vec;
    }

    Vector2i& operator+=(const Vector2i& rhs)
    {
        *this = *this + rhs;
        return *this;
    }

    int x;
    int y;
};
