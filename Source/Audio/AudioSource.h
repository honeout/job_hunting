#pragma once

#include <memory>
#include <xaudio2.h>
#include "Audio/AudioResource.h"
#include <unordered_map>

// �I�[�f�B�I�\�[�X
class AudioSource
{
public:
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);
	~AudioSource();

	// �Đ�
	void Play(bool loop);

	// ��~
	void Stop();

	// �{�����[���ύX
	void SetVolume(float volume) { sourceVoice->SetVolume(volume); }

	// ���x�ύX
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

	// �Đ�
	void Play(bool loop);

	// ��~
	void Stop();

	// �{�����[���ύX
	void SetVolume(float volume) { sourceVoice->SetVolume(volume); }

	// ���x�ύX
	void SetSpeed(float speed) { sourceVoice->SetFrequencyRatio(speed); }



private:
	IXAudio2SourceVoice* sourceVoice = nullptr;
	std::shared_ptr<AudioResource>	resource;
	// �Đ��@�\
	bool isPlaying;
};


class AudioSeSource
{
public:
	AudioSeSource(IXAudio2* xaudio);
	//AudioSeSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource,
	//	const std::string& name);
	virtual ~AudioSeSource();


	// �Đ�
	void Play(const std::string& name, bool loop = false, float volume = 1.0f);

	// ��~
	void Stop(const std::string& name);

	// �L�^
	void LoadSE( const char* filename,const std::string& name);
	//void LoadSE(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource,const std::string& name);

	// ���ݒ�
	//void SetMusic(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);

	// �{�����[���ύX
	void SetVolume(const std::string& name, float volume = 1.0f);
		//{ sourceVoice->SetVolume(volume); }

		// ���x�ύX
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

	// �Đ��@�\
	bool isPlaying = false;
};
