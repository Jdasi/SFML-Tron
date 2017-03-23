#include "RandomEngine.h"

std::random_device RandomEngine::rd_;
std::mt19937 RandomEngine::mt_(rd_());

// Generates a random int within the specified range.
int RandomEngine::rangei(int _min, int _max)
{
    std::uniform_int_distribution<int> random(_min, _max);
    return random(mt_);
}

// Generates a random float within the specified range.
float RandomEngine::rangef(float _min, float _max)
{
    std::uniform_real_distribution<float> random(_min, _max);
    return random(mt_);
}
