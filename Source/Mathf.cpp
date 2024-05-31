#include "Mathf.h"
#include <stdlib.h>

// 線形補完
float Mathf::Lerp(float a, float b, float t)
{
    return a * (1.0f - t) + (b * t);
}

float Mathf::RandomRange(float min, float max)
{
    // 0.0~1.0の間までのランダム値
    float value = static_cast<float>(rand()) / RAND_MAX;

    // min~maxまでのランダム値に変換
    // 0~1までをminでそこさげする。
    return min + (max - min) * value;
}
