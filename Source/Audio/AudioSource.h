#pragma once

#include <memory>
#include <xaudio2.h>
#include "Audio/AudioResource.h"
#include "Audio\AudioParam.h"
#include <unordered_map>

// �I�[�f�B�I�\�[�X
class AudioSource
{
public:
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource,AudioParam param);
	~AudioSource();

	// �Đ�
	void Play();

	// ��~
	void Stop();

	// �I�[�f�B�I���A�N�e�B�u�ł��邩
	bool IsAudioActive();

	// �o�b�t�@����ɂ���
	void ClearBuffers();

	// �{�����[���ύX
	void SetVolume(float volume) { sourceVoice->SetVolume(volume); }
	void GetVolume(float* v) { sourceVoice->GetVolume(v); }
	float GetVolume() { float v{}; sourceVoice->GetVolume(&v); return v; }

	// ���x�ύX
	void SetSpeed(float speed) { sourceVoice->SetFrequencyRatio(speed); }
#ifdef _DEBUG
	// �f�o�b�O
	void DebugDrawGUI();
#endif // DEBUG

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
//
//class AudioBgmSource 
//{
//public:
//	AudioBgmSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);
//	~AudioBgmSource();
//
//	// �Đ�
//	void Play(bool loop);
//
//	// ��~
//	void Stop();
//
//	// �{�����[���ύX
//	void SetVolume(float volume) { sourceVoice->SetVolume(volume); }
//
//	// ���x�ύX
//	void SetSpeed(float speed) { sourceVoice->SetFrequencyRatio(speed); }
//
//
//
//private:
//	IXAudio2SourceVoice* sourceVoice = nullptr;
//	std::shared_ptr<AudioResource>	resource;
//	// �Đ��@�\
//	bool isPlaying;
//};
//
//
//class AudioSeSource
//{
//public:
//	AudioSeSource(IXAudio2* xaudio);
//	//AudioSeSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource,
//	//	const std::string& name);
//	virtual ~AudioSeSource();
//
//
//	// �Đ�
//	void Play(const std::string& name, bool loop = false, float volume = 1.0f);
//
//	// ��~
//	void Stop(const std::string& name);
//
//	// �L�^
//	void LoadSE( const char* filename,const std::string& name);
//	//void LoadSE(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource,const std::string& name);
//
//	// ���ݒ�
//	//void SetMusic(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);
//
//	// �{�����[���ύX
//	void SetVolume(const std::string& name, float volume = 1.0f);
//		//{ sourceVoice->SetVolume(volume); }
//
//		// ���x�ύX
//	void SetSpeed(const std::string& name, float speed = 1.0f);
//	//{ sourceVoice->SetFrequencyRatio(speed); }
//
//
//
//private:
//	//IXAudio2SourceVoice* sourceVoice = nullptr;
//	//std::shared_ptr<AudioResource>	resource;
//	IXAudio2* xaudio;
//
//	std::unordered_map<std::string, IXAudio2SourceVoice*> voices;
//	std::unordered_map<std::string, XAUDIO2_BUFFER >buffers;
//	//std::unordered_map<std::string, std::shared_ptr<AudioResource>>	resources;
//	//std::unordered_map<std::string, IXAudio2* >	xaudio;
//
//	// �Đ��@�\
//	bool isPlaying = false;
//};
