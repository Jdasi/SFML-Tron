#pragma once

struct Vector2f
{
    Vector2f()
        : x(0)
        , y(0)
    {
    }

    Vector2f(float _x, float _y)
        : x(_x)
        , y(_y)
    {
    }

    Vector2f& operator=(Vector2f rhs)
    {
        this->x = rhs.x;
        this->y = rhs.y;

        return *this;
    }

    Vector2f operator +(const Vector2f& rhs) const
    {
        Vector2f vec = *this;

        vec.x += rhs.x;
        vec.y += rhs.y;

        return vec;
    }

    Vector2f& operator +=(const Vector2f& rhs)
    {
        *this = *this + rhs;
        return *this;
    }

    float x;
    float y;
};
