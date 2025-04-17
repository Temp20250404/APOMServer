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
    // ���� ����(rng)�� static���� ����  
    // = ���α׷� ���� �� �� ���� �õ�(seed) ����  
    // = ���� ȣ�� ������ ������ ������ �����Ͽ� ���� �� ǰ�� ����  
    // ----------------------------------------------------------------------  
    static std::mt19937_64 rng{ std::random_device{}() };

    // ----------------------------------------------------------------------  
    // �յ� �Ǽ� ���� ����  
    // = [minValue, maxValue) �������� �Ǽ��� ����  
    // = minValue > maxValue�� ��� ���� ������� ������ ȣ�� ���� �˻� �ʿ�  
    // ----------------------------------------------------------------------  
    std::uniform_real_distribution<float> dist(minValue, maxValue);

    // ----------------------------------------------------------------------  
    // ������ ���� ���� �� ��ȯ  
    // ----------------------------------------------------------------------  
    return dist(rng);
}
