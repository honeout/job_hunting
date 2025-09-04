#pragma once
#include <DirectXMath.h>

// 浮動少数算術
class Mathf
{
public:

    Mathf() {
        timeElapsed = 0.0f;
        timeElapsedValue = 1.0f;
        timeElapsedMin = 0.0f;
    };
    ~Mathf() {};

    // 線形補完
    static float Lerp(float a, float b, float t);

    // 指定範囲のランダム値を計算する
    static float RandomRange(float min, float max);

    // 経過時間
    bool UpdateElapsedTime(float timeMax, float elapsedTime);

    static DirectX::XMFLOAT3 Scale(DirectX::XMFLOAT3 scale, float number);

    // 経過時間セット
    void SetTimeElapsed(float timeElapsed) { this->timeElapsed = timeElapsed; }
    // 経過時間ゲット
    float GetTimeElapsed() const { return timeElapsed; }
    // 経過時間最低値
    float GetTimeElapsedMin() const { return timeElapsedMin; }
private:
    // 経過時間用
    float timeElapsed;
    float timeElapsedValue;

    float timeElapsedMin;
};
