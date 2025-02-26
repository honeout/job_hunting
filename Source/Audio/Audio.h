#pragma once

#include <xaudio2.h>
#include "Audio/AudioSource.h"
#include "AudioParam.h"

// �I�[�f�B�I
class Audio
{
public:
	Audio();
	~Audio();


public:
	// �C���X�^���X�擾
	//static Audio& Instance() { return *instance; }

	static Audio& Instance()
	{
		// �l�X�ȏ��Ŏ��o������������V���O���g���ɂ���
		static Audio instance;
		return instance;
	}

	// �Đ��J�n
	void Play(AudioParam param);


	// �Đ���~
	void Stop(AudioParam param);


	// �����̓z���~
	void Stop(std::string filename);

	// �S�ẴI�[�f�B�I���~����
	void AllStop();
	// �S�ẴI�[�f�B�I���Đ�������
	void AllStart();
	// �S�Ă̋Ȃ��폜����
	void AllClear();


	void Update();
#ifdef _DEBUG

	void DebugDrawGUI();
#endif // DEBUG
	//// �I�[�f�B�I�\�[�X�ǂݍ���
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
