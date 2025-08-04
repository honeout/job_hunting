#pragma once
#include <string>

struct AudioParam
{
    // 音の名前
    std::string filename{};
    // 音の大きさ
    float volume = 1.0f;
    // 音の変化速度
    float fadeSpeed = 0.01f;
    // 音の最大値
    float volumeMax = 1.0f;
    // 音の最低値
    float volumeMin = 0.0f;
    
    // ループするか
    bool loop = false;
};
