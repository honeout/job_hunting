#pragma once
#include <string>

struct AudioParam
{
    std::string filename{};
    float volume = 1.0f;
    bool loop = false;
};
