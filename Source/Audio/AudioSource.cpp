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


	filename = param.filename;

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


	HRESULT hr = sourceVoice->Start();
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

// 停止
void AudioSource::Stop()
{
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

