#pragma once

#include <memory>
#include <xaudio2.h>
#include "Audio/AudioResource.h"
#include "Component.h"

// オーディオソース
class AudioSource
{
public:
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);
	~AudioSource();

	// 再生
	void Play(bool loop);

	// 停止
	void Stop();

	// ボリューム変更
	void SetVolume(float volume) { sourceVoice->SetVolume(volume); }

	// 速度変更
	void SetSpeed(float speed) { sourceVoice->SetFrequencyRatio(speed); }

	

private:
	IXAudio2SourceVoice*			sourceVoice = nullptr;
	std::shared_ptr<AudioResource>	resource;
};

class AudioBgmSource 
{
public:
	AudioBgmSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);
	~AudioBgmSource();

	// 再生
	void Play(bool loop);

	// 停止
	void Stop();

	// ボリューム変更
	void SetVolume(float volume) { sourceVoice->SetVolume(volume); }

	// 速度変更
	void SetSpeed(float speed) { sourceVoice->SetFrequencyRatio(speed); }



private:
	IXAudio2SourceVoice* sourceVoice = nullptr;
	std::shared_ptr<AudioResource>	resource;
	// 再生機能
	bool isPlaying;
};


class AudioSeSource : public Component
{
public:
	AudioSeSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);
	virtual ~AudioSeSource();

	// 名前取得
	const char* GetName() const override { return "Audio"; }

	// 再生
	void Play(bool loop);

	// 停止
	void Stop();

	// 音設定
	void SetMusic(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);

	// ボリューム変更
	void SetVolume(float volume) { sourceVoice->SetVolume(volume); }

	// 速度変更
	void SetSpeed(float speed) { sourceVoice->SetFrequencyRatio(speed); }



private:
	IXAudio2SourceVoice* sourceVoice = nullptr;
	std::shared_ptr<AudioResource>	resource;

	// 再生機能
	bool isPlaying;
};
