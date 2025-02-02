#include "AudioResourceManager.h"

std::shared_ptr<AudioResource> AudioResourceManager::LoadAudioResource(const char* filename, const char* keyName)
{
    AudioMap::iterator it = audios.find(filename);
    if (it != audios.end())
    {
        // リンクが斬れていない場合
        if (it->second.expired() == false)
        {
            // std::shared_ptrを作って返す
            return it->second.lock();
        }
    }

    // 新規リソース作成＆読み込み
    std::shared_ptr<AudioResource> audio;
    audio = std::make_shared<AudioResource>(filename);

    audios[keyName] = std::weak_ptr<AudioResource>(audio);
    return audio;
}