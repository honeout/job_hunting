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

private:
	std::vector<AudioSource*> audio_source_pool;

private:

	IXAudio2*				xaudio = nullptr;
	IXAudio2MasteringVoice* masteringVoice = nullptr;
};
