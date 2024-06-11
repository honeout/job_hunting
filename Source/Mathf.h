#pragma once
#include <DirectXMath.h>

// ���������Z�p
class Mathf
{
public:
    // ���`�⊮
    static float Lerp(float a, float b, float t);

    // �w��͈͂̃����_���l���v�Z����
    static float RandomRange(float min, float max);

    static DirectX::XMFLOAT3 Scale(DirectX::XMFLOAT3 scale, float number);
};
