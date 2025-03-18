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

	// ���O�l��
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
