#include "Misc.h"
#include <imgui.h>
#include "Audio\Audio.h"
#include "Audio\AudioSource.h"

// コンストラクタ
AudioSource::AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource, AudioParam param)
	: resource(resource)
{
	HRESULT hr;
	
	// ソースボイスを生成
	hr = xaudio->CreateSourceVoice(&sourceVoice, &resource->GetWaveFormat());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ソースボイスにデータを送信
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = resource->GetAudioBytes();
	buffer.pAudioData = resource->GetAudioData();
	buffer.LoopCount = param.loop ? XAUDIO2_LOOP_INFINITE : 0;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	SetVolume(param.volume);

	sourceVoice->SubmitSourceBuffer(&buffer);

#ifdef _DEBUG
	filename = param.filename;
#endif // DEBUG
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
void AudioSource::Play()
{
	Stop();

	//// ソースボイスにデータを送信
	//XAUDIO2_BUFFER buffer = { 0 };
	//buffer.AudioBytes = resource->GetAudioBytes();
	//buffer.pAudioData = resource->GetAudioData();
	//buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
	//buffer.Flags = XAUDIO2_END_OF_STREAM;
	//
	//sourceVoice->SubmitSourceBuffer(&buffer);

	HRESULT hr = sourceVoice->Start();
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	//sourceVoice->SetVolume(1.0f);
}

// 停止
void AudioSource::Stop()
{
	//sourceVoice->FlushSourceBuffers();
	sourceVoice->Stop();
}

bool AudioSource::IsAudioActive()
{
	XAUDIO2_VOICE_STATE state;
	this->sourceVoice->GetState(&state);

	// 再生待ちのバッファが存在するか
	return(0 < state.BuffersQueued);
}

void AudioSource::ClearBuffers()
{
	this->sourceVoice->Stop();
	this->sourceVoice->FlushSourceBuffers();
}

#ifdef _DEBUG

void AudioSource::DebugDrawGUI()
{
	std::string label;
	label = this->filename + "##Audio" + std::to_string(id);
	if (ImGui::CollapsingHeader(label.c_str()))
	{
		float volume{};
		this->sourceVoice->GetVolume(&volume);

		label = "volume##Audio" + std::to_string(id);
		if (ImGui::SliderFloat(label.c_str(), &volume, 0.0f, 1.0f))
		{
			this->sourceVoice->SetVolume(volume);
		}
		label = "Play##Audio" + std::to_string(id);
		if (ImGui::Button(label.c_str()))
		{
			Play();
		}
		ImGui::SameLine();
		label = "Stop##Audio" + std::to_string(id);
		if (ImGui::Button(label.c_str()))
		{
			Stop();
		}
	}
}

#endif // DEBUG
//
//// BGM
//AudioBgmSource::AudioBgmSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource)
//{
//
//	HRESULT hr;
//
//	// ソースボイスを生成
//	hr = xaudio->CreateSourceVoice(&sourceVoice, &resource->GetWaveFormat());
//	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//}
//
//AudioBgmSource::~AudioBgmSource()
//{
//	if (sourceVoice != nullptr)
//	{
//		sourceVoice->DestroyVoice();
//		sourceVoice = nullptr;
//	}
//}
//
//void AudioBgmSource::Play(bool loop)
//{
//	// ソースボイスにデータを送信
//	XAUDIO2_BUFFER buffer = { 0 };
//	buffer.AudioBytes = resource->GetAudioBytes();
//	buffer.pAudioData = resource->GetAudioData();
//	buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
//	buffer.Flags = XAUDIO2_END_OF_STREAM;
//
//	sourceVoice->SubmitSourceBuffer(&buffer);
//
//	HRESULT hr = sourceVoice->Start();
//	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//	sourceVoice->SetVolume(1.0f);
//
//}
//
//void AudioBgmSource::Stop()
//{
//	sourceVoice->FlushSourceBuffers();
//	sourceVoice->Stop();
//}
//// SE
//AudioSeSource::AudioSeSource(IXAudio2* xaudio)
//{
//
//
//	//HRESULT hr;
//
//	//IXAudio2SourceVoice* sourceVoice = nullptr;
//
//	//// ソースボイスを生成
//	//hr = xaudio->CreateSourceVoice(&sourceVoice, &resource->GetWaveFormat());
//	//_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//	//// ソースボイスにデータを送信
//	//XAUDIO2_BUFFER buffer = { 0 };
//	//buffer.AudioBytes = resource->GetAudioBytes();
//	//buffer.pAudioData = resource->GetAudioData();
//	////buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
//	//buffer.Flags = XAUDIO2_END_OF_STREAM;
//	//
//
//	//voices[name] = sourceVoice;
//	//buffers[name] = buffer;
//	//this->resource = resource;
//	//this->xaudio = xaudio;
//
//	//isPlaying = false;
//	this->xaudio = xaudio;
//}
//
//AudioSeSource::~AudioSeSource()
//{
//	//for (auto& [name, voice] : voices)
//	//{
//	//	voice->DestroyVoice();
//	//}
//	// 音関係
//	if (xaudio != nullptr)
//	{
//		xaudio = nullptr;
//	}
//
//	//if (sourceVoice != nullptr)
//	//{
//	//	sourceVoice->DestroyVoice();
//	//	sourceVoice = nullptr;
//	//}
//}
//
//void AudioSeSource::Play(const std::string& name,bool loop,float volume)
//{
//
//	if (voices.find(name) != voices.end())
//	{
//
//		
//		//HRESULT hr;
//		//// ソースボイスを生成
//  //      hr = xaudio->CreateSourceVoice(&sourceVoice, &resouces[name]->GetWaveFormat());
//  //      _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//		//// ソースボイスにデータを送信
//  //      XAUDIO2_BUFFER buffer = { 0 };
//  //      buffer.AudioBytes = resource->GetAudioBytes();
//  //      buffer.pAudioData = resource->GetAudioData();
//  //      //buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
//  //      buffer.Flags = XAUDIO2_END_OF_STREAM;
//
//		buffers[name].LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
//		auto sourceVoice = voices[name];
//		sourceVoice->SubmitSourceBuffer(&buffers[name]);
//		HRESULT hr = sourceVoice->Start();
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//		sourceVoice->SetVolume(volume);
//	}
//	//if (!isPlaying && sourceVoice != nullptr)
//	//{
//	//	HRESULT hr = sourceVoice->Start();
//	//	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//	//	sourceVoice->SetVolume(1.0f);
//
//	//	isPlaying = true;
//	//}
//}
//
//void AudioSeSource::Stop(const std::string& name )
//{
//
//	if (voices.find(name) != voices.end())
//	{
//		
//		auto sourceVoice = voices[name];
//
//		sourceVoice->Stop();
//		sourceVoice->FlushSourceBuffers();
//	}
//	//if (isPlaying)
//	//{
//	//	sourceVoice->Stop();
//	//	sourceVoice->FlushSourceBuffers();
//	//	isPlaying = false;
//	//}
//}
//
//void AudioSeSource::LoadSE(const char* filename,const std::string& name)
//{
//
//
//	IXAudio2SourceVoice* sourceVoice = nullptr;
//	std::shared_ptr<AudioResource>	resource;
//	resource = Audio::Instance().LoadAudioSourceSe(filename);
//
//	HRESULT hr;
//	// ソースボイスを生成
//	hr = xaudio->CreateSourceVoice(&sourceVoice, &resource->GetWaveFormat());
//	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//	// ソースボイスにデータを送信
//	XAUDIO2_BUFFER buffer = { 0 };
//	buffer.AudioBytes = resource->GetAudioBytes();
//	buffer.pAudioData = resource->GetAudioData();
//	//buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
//	buffer.Flags = XAUDIO2_END_OF_STREAM;
//
//
//	voices[name] = sourceVoice;
//	buffers[name] = buffer;
//	//this->resource = resource;
//}
//
//void AudioSeSource::SetVolume(const std::string& name, float volume)
//{
//	if (voices.find(name) != voices.end())
//	{
//		auto sourceVoice = voices[name];
//
//		sourceVoice->SetVolume(volume);
//	}
//}
//
//void AudioSeSource::SetSpeed(const std::string& name, float speed)
//{
//	if (voices.find(name) != voices.end())
//	{
//		auto sourceVoice = voices[name];
//
//		sourceVoice->SetFrequencyRatio(speed);
//	}
//}
//
////void AudioSeSource::SetMusic(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource)
////{
////}
