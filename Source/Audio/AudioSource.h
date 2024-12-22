#pragma once

#include <memory>
#include <xaudio2.h>
#include "Audio/AudioResource.h"
#include "Component.h"

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


class AudioSeSource : public Component
{
public:
	AudioSeSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);
	virtual ~AudioSeSource();

	// ���O�擾
	const char* GetName() const override { return "Audio"; }

	// �Đ�
	void Play(bool loop);

	// ��~
	void Stop();

	// ���ݒ�
	void SetMusic(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);

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
