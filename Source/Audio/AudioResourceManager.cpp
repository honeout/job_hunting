#include "AudioResourceManager.h"

std::shared_ptr<AudioResource> AudioResourceManager::LoadAudioResource(const char* filename, const char* keyName)
{
    AudioMap::iterator it = audios.find(filename);
    if (it != audios.end())
    {
        // �����N���a��Ă��Ȃ��ꍇ
        if (it->second.expired() == false)
        {
            // std::shared_ptr������ĕԂ�
            return it->second.lock();
        }
    }

    // �V�K���\�[�X�쐬���ǂݍ���
    std::shared_ptr<AudioResource> audio;
    audio = std::make_shared<AudioResource>(filename);

    audios[keyName] = std::weak_ptr<AudioResource>(audio);
    return audio;
}