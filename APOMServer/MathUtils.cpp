#include "MathUtils.h"
#include <cmath>

float CalculateDistance(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float dz = z1 - z2;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}