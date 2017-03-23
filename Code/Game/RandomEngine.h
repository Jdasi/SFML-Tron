#pragma once
#include <random>

// Simple static class for generating random numbers.
class RandomEngine
{
public:
    RandomEngine() = delete;
    ~RandomEngine() = delete;
    
    static int rangei(int _min, int _max);
    static float rangef(float _min, float _max);

private:
    static std::random_device rd_;
    static std::mt19937 mt_;
    
};
