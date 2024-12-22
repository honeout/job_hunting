#include "Misc.h"
#include "Audio/AudioSource.h"

// コンストラクタ
AudioSource::AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource)
	: resource(resource)
{
	HRESULT hr;
	
	// ソースボイスを生成
	hr = xaudio->CreateSourceVoice(&sourceVoice, &resource->GetWaveFormat());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

// デストラクタ
AudioSource::~AudioSource()
{
	if (sourceVoice != nullptr)
	{
		sourceVoice->DestroyVoice();
		sourceVoice = nullptr;
	}
}

// 再生
void AudioSource::Play(bool loop)
{
	Stop();

	// ソースボイスにデータを送信
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	
	sourceVoice->SubmitSourceBuffer(&buffer);

	HRESULT hr = sourceVoice->Start();
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	sourceVoice->SetVolume(1.0f);
}

// 停止
void AudioSource::Stop()
{
	sourceVoice->FlushSourceBuffers();
	sourceVoice->Stop();
}

// BGM
AudioBgmSource::AudioBgmSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource)
{

	HRESULT hr;
	
	// ソースボイスを生成
	hr = xaudio->CreateSourceVoice(&sourceVoice, &resource->GetWaveFormat());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ソースボイスにデータを送信
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	sourceVoice->SubmitSourceBuffer(&buffer);

	isPlaying = false;
}

AudioBgmSource::~AudioBgmSource()
{
	if (sourceVoice != nullptr)
	{
		sourceVoice->DestroyVoice();
		sourceVoice = nullptr;
	}
}

void AudioBgmSource::Play(bool loop)
{
	if (!isPlaying)
	{
		HRESULT hr = sourceVoice->Start();
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		sourceVoice->SetVolume(1.0f);

		isPlaying = true;
	}

}

void AudioBgmSource::Stop()
{
	if (isPlaying)
	{
		sourceVoice->Stop();
		sourceVoice->FlushSourceBuffers();
		isPlaying = false;
	}
}
// SE
AudioSeSource::AudioSeSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource)
{
	HRESULT hr;

	// ソースボイスを生成
	hr = xaudio->CreateSourceVoice(&sourceVoice, &resource->GetWaveFormat());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ソースボイスにデータを送信
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	sourceVoice->SubmitSourceBuffer(&buffer);

	isPlaying = false;
}

AudioSeSource::~AudioSeSource()
{
	if (sourceVoice != nullptr)
	{
		sourceVoice->DestroyVoice();
		sourceVoice = nullptr;
	}
}

void AudioSeSource::Play(bool loop)
{
	std::weak_ptr<Actor> actor = GetActor();


	if (!isPlaying && sourceVoice != nullptr)
	{
		HRESULT hr = sourceVoice->Start();
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		sourceVoice->SetVolume(1.0f);

		isPlaying = true;
	}
}

void AudioSeSource::Stop()
{
	if (isPlaying)
	{
		sourceVoice->Stop();
		sourceVoice->FlushSourceBuffers();
		isPlaying = false;
	}
}

void AudioSeSource::SetMusic(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource)
{
}
