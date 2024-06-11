#include "Mathf.h"
#include <stdlib.h>

// ���`�⊮
float Mathf::Lerp(float a, float b, float t)
{
    return a * (1.0f - t) + (b * t);
}

float Mathf::RandomRange(float min, float max)
{
    // 0.0~1.0�̊Ԃ܂ł̃����_���l
    float value = static_cast<float>(rand()) / RAND_MAX;

    // min~max�܂ł̃����_���l�ɕϊ�
    // 0~1�܂ł�min�ł�����������B
    return min + (max - min) * value;
}

DirectX::XMFLOAT3 Mathf::Scale(DirectX::XMFLOAT3 scale, float number)
{
    return {scale.x* number,scale.y * number, scale.z * number, };
}
