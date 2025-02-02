#include "Misc.h"
#include "Audio/Audio.h"
#include "Audio\AudioResourceManager.h"

Audio* Audio::instance = nullptr;

// �R���X�g���N�^
Audio::Audio()
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;

	HRESULT hr;

	// COM�̏�����
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	UINT32 createFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	//createFlags |= XAUDIO2_DEBUG_ENGINE;
#endif

	// XAudio������
	hr = XAudio2Create(&xaudio, createFlags);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// �}�X�^�����O�{�C�X����
	hr = xaudio->CreateMasteringVoice(&masteringVoice);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	// �}�X�^�[�{�C�X�̃{�����[���ݒ�
	masteringVoice->SetVolume(0.5f);
}

// �f�X�g���N�^
Audio::~Audio()
{

	// �I�[�f�B�I�\�[�X�̍폜
	{
		for (AudioSource* source : this->audio_source_pool)
		{
			if (source != nullptr)
			{
				delete source;
				source = nullptr;
			}
		}
	}

	// �}�X�^�����O�{�C�X�j��
	if (masteringVoice != nullptr)
	{
		masteringVoice->DestroyVoice();
		masteringVoice = nullptr;
	}

	// XAudio�I����
	if (xaudio != nullptr)
	{
		xaudio->Release();
		xaudio = nullptr;
	}

	// COM�I����
	CoUninitialize();
}

void Audio::Play(AudioParam param)
{
	// ���\�[�X�쐬
	AudioResourceManager& audioResourceManager = AudioResourceManager::Instance();
	if (&audioResourceManager) return;
	std::shared_ptr<AudioResource> resource = audioResourceManager.LoadAudioResource(param.filename.c_str(),param.keyName.c_str());
	// �I�[�f�B�I�\�[�X�쐬
	AudioSource* audio = this->audio_source_pool.emplace_back(new AudioSource(this->xaudio, resource, param));

	audio->Play();
}

void Audio::AllStop()
{
	for (AudioSource* audio_source : this->audio_source_pool)
	{
		audio_source->Stop();
	}
}

void Audio::AllStart()
{
	for (AudioSource* audio_source : this->audio_source_pool)
	{
		audio_source->Play();
	}
}

void Audio::AllClear()
{
	for (AudioSource* audio_source : this->audio_source_pool)
	{
		audio_source->ClearBuffers();
	}
}

void Audio::Update()
{
	// �Đ��I�������v�f���폜����
	for (size_t i = 0; i < audio_source_pool.size(); ++i)
	{
		AudioSource* audio_source = audio_source_pool[i];

		if (!audio_source) continue;
		// �I�[�f�B�I���L���Ȃ�X�L�b�v
		if (audio_source->IsAudioActive()) continue;

		auto it = std::find(this->audio_source_pool.begin(), this->audio_source_pool.end(), audio_source);
		if (it == this->audio_source_pool.end()) continue;

		// �I�[�f�B�I�̍폜
		delete audio_source;
		audio_source = nullptr;
		this->audio_source_pool.erase(it);
	}
}

//
//// �I�[�f�B�I�\�[�X�ǂݍ���
//std::unique_ptr<AudioSource> Audio::LoadAudioSource(const char* filename)
//{
//	std::shared_ptr<AudioResource> resource = std::make_shared<AudioResource>(filename);
//	return std::make_unique<AudioSource>(xaudio, resource);
//}
////// �I�[�f�B�I�\�[�X�ǂݍ���
////std::unique_ptr<AudioSeSource> Audio::LoadAudioSource(const char* filename,
////	const std::string& name)
////{
////	std::shared_ptr<AudioResource> resource = std::make_shared<AudioResource>(filename);
////	return std::make_unique<AudioSeSource>(xaudio, resource, name);
////	
////}
//// �I�[�f�B�I�\�[�X�ǂݍ���
//std::unique_ptr<AudioSeSource> Audio::LoadAudioSource()
//{
//	return  std::make_unique<AudioSeSource>(xaudio);
//}
//
//std::shared_ptr<AudioResource> Audio::LoadAudioSourceSe(const char* filename)
//{
//	std::shared_ptr<AudioResource> resource = std::make_shared<AudioResource>(filename);
//	return resource;
//}

//// �I�[�f�B�I�\�[�X�ǂݍ���
//std::unique_ptr<AudioSeSource> Audio::LoadAudioSource(const char* filename,
//	const std::string& name)
//{
//	std::shared_ptr<AudioResource> resource = std::make_shared<AudioResource>(filename);
//	return std::make_unique<AudioSeSource>(xaudio, resource, name);
//
//}
