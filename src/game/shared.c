/// Copyright (c) 2024 Bartosz Lenart

#include <math.h>
#include "game.h"

inline float randInRange(float a, float b)
{
    float r = (float)rand() / (float)RAND_MAX;
    return a + r * (b - a); 
}

