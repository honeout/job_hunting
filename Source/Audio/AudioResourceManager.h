#pragma once
#include <map>
#include <memory>
#include <string>
#include "Audio\AudioResource.h"

class AudioResourceManager
{
public:
    // �C���X�^���X�擾
    static AudioResourceManager& Instance() { 
        static AudioResourceManager audioResourceManager;
        return audioResourceManager;
    }

    AudioResourceManager() {};
    ~AudioResourceManager() {}

    // model���\�[�X�ǂݍ���
    std::shared_ptr<AudioResource> LoadAudioResource(const char* filename);

private:
    using AudioMap = std::map<std::string, std::weak_ptr<AudioResource>>;

    AudioMap audios;
};
