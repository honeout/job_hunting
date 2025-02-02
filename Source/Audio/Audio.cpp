#include "Misc.h"
#include "Audio/Audio.h"
#include "Audio\AudioResourceManager.h"

Audio* Audio::instance = nullptr;

// コンストラクタ
Audio::Audio()
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;

	HRESULT hr;

	// COMの初期化
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	UINT32 createFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	//createFlags |= XAUDIO2_DEBUG_ENGINE;
#endif

	// XAudio初期化
	hr = XAudio2Create(&xaudio, createFlags);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// マスタリングボイス生成
	hr = xaudio->CreateMasteringVoice(&masteringVoice);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	// マスターボイスのボリューム設定
	masteringVoice->SetVolume(0.5f);
}

// デストラクタ
Audio::~Audio()
{

	// オーディオソースの削除
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

	// マスタリングボイス破棄
	if (masteringVoice != nullptr)
	{
		masteringVoice->DestroyVoice();
		masteringVoice = nullptr;
	}

	// XAudio終了化
	if (xaudio != nullptr)
	{
		xaudio->Release();
		xaudio = nullptr;
	}

	// COM終了化
	CoUninitialize();
}

void Audio::Play(AudioParam param)
{
	// リソース作成
	AudioResourceManager& audioResourceManager = AudioResourceManager::Instance();
	if (&audioResourceManager) return;
	std::shared_ptr<AudioResource> resource = audioResourceManager.LoadAudioResource(param.filename.c_str(),param.keyName.c_str());
	// オーディオソース作成
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
	// 再生終了した要素を削除する
	for (size_t i = 0; i < audio_source_pool.size(); ++i)
	{
		AudioSource* audio_source = audio_source_pool[i];

		if (!audio_source) continue;
		// オーディオが有効ならスキップ
		if (audio_source->IsAudioActive()) continue;

		auto it = std::find(this->audio_source_pool.begin(), this->audio_source_pool.end(), audio_source);
		if (it == this->audio_source_pool.end()) continue;

		// オーディオの削除
		delete audio_source;
		audio_source = nullptr;
		this->audio_source_pool.erase(it);
	}
}

//
//// オーディオソース読み込み
//std::unique_ptr<AudioSource> Audio::LoadAudioSource(const char* filename)
//{
//	std::shared_ptr<AudioResource> resource = std::make_shared<AudioResource>(filename);
//	return std::make_unique<AudioSource>(xaudio, resource);
//}
////// オーディオソース読み込み
////std::unique_ptr<AudioSeSource> Audio::LoadAudioSource(const char* filename,
////	const std::string& name)
////{
////	std::shared_ptr<AudioResource> resource = std::make_shared<AudioResource>(filename);
////	return std::make_unique<AudioSeSource>(xaudio, resource, name);
////	
////}
//// オーディオソース読み込み
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

//// オーディオソース読み込み
//std::unique_ptr<AudioSeSource> Audio::LoadAudioSource(const char* filename,
//	const std::string& name)
//{
//	std::shared_ptr<AudioResource> resource = std::make_shared<AudioResource>(filename);
//	return std::make_unique<AudioSeSource>(xaudio, resource, name);
//
//}
