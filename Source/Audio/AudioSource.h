#pragma once

#include <memory>
#include <xaudio2.h>
#include "Audio/AudioResource.h"
#include <unordered_map>

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


class AudioSeSource
{
public:
	AudioSeSource(IXAudio2* xaudio);
	//AudioSeSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource,
	//	const std::string& name);
	virtual ~AudioSeSource();


	// 再生
	void Play(const std::string& name, bool loop = false, float volume = 1.0f);

	// 停止
	void Stop(const std::string& name);

	// 記録
	void LoadSE( const char* filename,const std::string& name);
	//void LoadSE(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource,const std::string& name);

	// 音設定
	//void SetMusic(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);

	// ボリューム変更
	void SetVolume(const std::string& name, float volume = 1.0f);
		//{ sourceVoice->SetVolume(volume); }

		// 速度変更
	void SetSpeed(const std::string& name, float speed = 1.0f);
	//{ sourceVoice->SetFrequencyRatio(speed); }



private:
	//IXAudio2SourceVoice* sourceVoice = nullptr;
	//std::shared_ptr<AudioResource>	resource;
	IXAudio2* xaudio;

	std::unordered_map<std::string, IXAudio2SourceVoice*> voices;
	std::unordered_map<std::string, XAUDIO2_BUFFER >buffers;
	//std::unordered_map<std::string, std::shared_ptr<AudioResource>>	resources;
	//std::unordered_map<std::string, IXAudio2* >	xaudio;

	// 再生機能
	bool isPlaying = false;
};
