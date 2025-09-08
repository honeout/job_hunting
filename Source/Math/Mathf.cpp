#include "Mathf.h"
#include <stdlib.h>

// 線形補完
float Mathf::Lerp(float a, float b, float t)
{
    // (a(変化する値),b(最終的な値),t(これだけ進))
    return a + (b - a) * t;
    //return a * (1.0f - t) + (b * t);
}
// 範囲指定
float Mathf::RandomRange(float min, float max)
{
    // 0.0~1.0の間までのランダム値
    float value = static_cast<float>(rand()) / RAND_MAX;

    // min~maxまでのランダム値に変換
    // 0~1までをminでそこさげする。
    return min + (max - min) * value;
}

// 円の範囲指定

// 経過時間
bool Mathf::UpdateElapsedTime(float timeMax, float elapsedTime)
{
    if (timeElapsed >= timeMax)
    {
        timeElapsed = timeElapsedMin;
        return true;
    }
    timeElapsed += timeElapsedValue * elapsedTime;
    return false;
}
// 徐々に縮める
float Mathf::LinearInterpolate(float currentScale, float maxScale, float lerpSpeed, float elapsedTime)
{
    // 線形補完に使う割合
    float interpolationRate = lerpSpeed * elapsedTime;

    // 線形補完で徐々に縮める
    return Lerp(currentScale, maxScale, interpolationRate);
}

DirectX::XMFLOAT3 Mathf::Scale(DirectX::XMFLOAT3 scale, float number)
{
    return {scale.x* number,scale.y * number, scale.z * number, };
}
