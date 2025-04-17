#include "MathUtils.h"
#include <cmath>

float CalculateDistance(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float dz = z1 - z2;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float RandomRange(float minValue, float maxValue)
{
    // ----------------------------------------------------------------------  
    // 난수 엔진(rng)을 static으로 선언  
    // = 프로그램 시작 시 한 번만 시드(seed) 생성  
    // = 이후 호출 때마다 동일한 엔진을 재사용하여 성능 및 품질 보장  
    // ----------------------------------------------------------------------  
    static std::mt19937_64 rng{ std::random_device{}() };

    // ----------------------------------------------------------------------  
    // 균등 실수 분포 생성  
    // = [minValue, maxValue) 범위에서 실수를 생성  
    // = minValue > maxValue일 경우 동작 보장되지 않으니 호출 전에 검사 필요  
    // ----------------------------------------------------------------------  
    std::uniform_real_distribution<float> dist(minValue, maxValue);

    // ----------------------------------------------------------------------  
    // 분포에 따른 랜덤 값 반환  
    // ----------------------------------------------------------------------  
    return dist(rng);
}
