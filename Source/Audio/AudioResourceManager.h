#pragma once
#include <map>
#include <memory>
#include <string>
#include "Audio\AudioResource.h"

class AudioResourceManager
{
public:
    // インスタンス取得
    static AudioResourceManager& Instance() { 
        static AudioResourceManager audioResourceManager;
        return audioResourceManager;
    }

    AudioResourceManager() {};
    ~AudioResourceManager() {}

    // modelリソース読み込み
    std::shared_ptr<AudioResource> LoadAudioResource(const char* filename);

private:
    using AudioMap = std::map<std::string, std::weak_ptr<AudioResource>>;

    AudioMap audios;
};
