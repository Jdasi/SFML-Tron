#pragma once

// Inherit from Noncopyable if a class should not be copyable.
class Noncopyable
{
public:
    Noncopyable() = default;
    virtual ~Noncopyable() = default;

    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator=(const Noncopyable &) = delete;
};
