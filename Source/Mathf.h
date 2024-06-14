#pragma once
#include <DirectXMath.h>

// 浮動少数算術
class Mathf
{
public:
    // 線形補完
    static float Lerp(float a, float b, float t);

    // 指定範囲のランダム値を計算する
    static float RandomRange(float min, float max);

    static DirectX::XMFLOAT3 Scale(DirectX::XMFLOAT3 scale, float number);
};
