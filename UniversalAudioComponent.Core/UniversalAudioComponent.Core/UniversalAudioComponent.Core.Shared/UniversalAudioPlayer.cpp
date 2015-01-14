#pragma once

#include "pch.h"
#include "UniversalAudioPlayer.h"

using namespace Platform;
using namespace Platform::Collections;
using namespace Microsoft::WRL;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation::Collections;
using namespace Concurrency;
using namespace UniversalAudioComponent;

UniversalAudioPlayer::UniversalAudioPlayer()
{
    HRESULT hr = XAudio2Create(&xAudio);

    if (FAILED(hr))
        ref new COMException(hr, "XAudio2Create failure");

    XAUDIO2_DEBUG_CONFIGURATION debugConfig = { 0 };
    debugConfig.TraceMask = XAUDIO2_LOG_DETAIL | XAUDIO2_LOG_WARNINGS;
    xAudio->SetDebugConfiguration(&debugConfig);

    hr = xAudio->CreateMasteringVoice(&masteringVoice);

    xAudio->StartEngine();

    this->reader = new RiffReader();
    this->dataCache = ref new Map<String^, AudioData^>();
}

void UniversalAudioPlayer::PlayMusic(AudioSample^ sample)
{
    auto data = this->reader->Read(sample->Buffer);

    HRESULT hr = xAudio->CreateSourceVoice(&musicVoice, data->waveFormat);

    XAUDIO2_BUFFER buffer = { 0 };
    buffer.AudioBytes = data->numberOfBytes;
    buffer.pAudioData = data->bytes;
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = 100;

    hr = musicVoice->SubmitSourceBuffer(&buffer);

    if (FAILED(hr))
        throw ref new COMException(hr, "SubmitSourceBuffer failure");

    musicVoice->Start(0);

}

void UniversalAudioPlayer::PlayEffect(AudioSample^ sample)
{
    AudioData^ data = nullptr;
    

    if (effectsVoice != nullptr)
    {
        effectsVoice->Stop();
        effectsVoice->FlushSourceBuffers();
        effectsVoice->DestroyVoice();
        effectsVoice = nullptr;
    }

    if (!this->dataCache->HasKey(sample->Name))
    {
        data = this->reader->Read(sample->Buffer);
        this->dataCache->Insert(sample->Name, data);
    }
    else
    {
        data = this->dataCache->Lookup(sample->Name);
    }

    HRESULT hr = xAudio->CreateSourceVoice(&effectsVoice, data->waveFormat);

    XAUDIO2_BUFFER buffer = { 0 };
    buffer.AudioBytes = data->numberOfBytes;
    buffer.pAudioData = data->bytes;
    buffer.Flags = XAUDIO2_END_OF_STREAM;

    hr = effectsVoice->SubmitSourceBuffer(&buffer);

    if (FAILED(hr))
        throw ref new COMException(hr, "SubmitSourceBuffer failure");

    effectsVoice->Start(0);
}