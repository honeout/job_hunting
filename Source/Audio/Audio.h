#pragma once

#include <xaudio2.h>
#include "Audio/AudioSource.h"

// オーディオ
class Audio
{
public:
	Audio();
	~Audio();

public:
	// インスタンス取得
	static Audio& Instance() { return *instance; }

	// オーディオソース読み込み
	std::unique_ptr<AudioSource> LoadAudioSource(const char* filename);
	std::unique_ptr<AudioSeSource> LoadAudioSource();

	std::shared_ptr<AudioResource> LoadAudioSourceSe(const char* filename);

	//std::unique_ptr<AudioSeSource> LoadAudioSource(const char* filename,
	//	const std::string& name);

private:
	static Audio*			instance;

	IXAudio2*				xaudio = nullptr;
	IXAudio2MasteringVoice* masteringVoice = nullptr;
};
