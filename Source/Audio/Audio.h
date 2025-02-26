#pragma once

#include <xaudio2.h>
#include "Audio/AudioSource.h"
#include "AudioParam.h"

// オーディオ
class Audio
{
public:
	Audio();
	~Audio();


public:
	// インスタンス取得
	//static Audio& Instance() { return *instance; }

	static Audio& Instance()
	{
		// 様々な所で取り出したいだからシングルトンにする
		static Audio instance;
		return instance;
	}

	// 再生開始
	void Play(AudioParam param);


	// 再生停止
	void Stop(AudioParam param);


	// 既存の奴を停止
	void Stop(std::string filename);

	// 全てのオーディオを停止する
	void AllStop();
	// 全てのオーディオを再生させる
	void AllStart();
	// 全ての曲を削除する
	void AllClear();


	void Update();
#ifdef _DEBUG

	void DebugDrawGUI();
#endif // DEBUG
	//// オーディオソース読み込み
	//std::unique_ptr<AudioSource> LoadAudioSource(const char* filename);
	//std::unique_ptr<AudioSeSource> LoadAudioSource();

	//std::shared_ptr<AudioResource> LoadAudioSourceSe(const char* filename);

	//std::unique_ptr<AudioSeSource> LoadAudioSource(const char* filename,
	//	const std::string& name);
private:
	std::vector<AudioSource*> audio_source_pool;

private:
	//static Audio*			instance;

	IXAudio2*				xaudio = nullptr;
	IXAudio2MasteringVoice* masteringVoice = nullptr;
};
