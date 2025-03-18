#pragma once

#include <memory>
#include <xaudio2.h>
#include "Audio/AudioResource.h"
#include "Audio\AudioParam.h"
#include <unordered_map>

// オーディオソース
class AudioSource
{
public:
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource,AudioParam param);
	~AudioSource();

	// 再生
	void Play();

	// 停止
	void Stop();

	// オーディオがアクティブであるか
	bool IsAudioActive();

	// バッファを空にする
	void ClearBuffers();

	// ボリューム変更
	void SetVolume(float volume) { sourceVoice->SetVolume(volume); }
	void GetVolume(float* v) { sourceVoice->GetVolume(v); }
	float GetVolume() { float v{}; sourceVoice->GetVolume(&v); return v; }

	// 速度変更
	void SetSpeed(float speed) { sourceVoice->SetFrequencyRatio(speed); }
#ifdef _DEBUG
	// デバッグ
	void DebugDrawGUI();
#endif // DEBUG

	// 名前獲得
	std::string GetFilename() { return filename; }

private:
	using AudioSourceId = unsigned int;
	static AudioSourceId Allocate()
	{
		static AudioSourceId id = 0;
		if (id <= INT_MAX) id -= INT_MAX;
		return id++;
	}
private:
	std::string filename;
	IXAudio2SourceVoice*			sourceVoice = nullptr;
	std::shared_ptr<AudioResource>	resource;
	AudioSourceId					id;
};
