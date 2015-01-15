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
    //this->runningVoices = new std::map<String^, IXAudio2SourceVoice*>();
}

bool UniversalAudioPlayer::IsPlaying(AudioSample^ sample)
{
    auto voice = this->runningVoices.find(sample->Name);

    return voice != this->runningVoices.end();
}

void UniversalAudioPlayer::Play(AudioSample^ sample)
{
    if (this->IsPlaying(sample))
    {
        return;
    }

    auto data = this->reader->Read(sample->Buffer);
    IXAudio2SourceVoice * voice = NULL;
    HRESULT hr = xAudio->CreateSourceVoice(&voice, data->waveFormat);

    if (FAILED(hr))
        throw ref new COMException(hr, "SubmitSourceBuffer failure");

    XAUDIO2_BUFFER buffer = { 0 };
    buffer.AudioBytes = data->numberOfBytes;
    buffer.pAudioData = data->bytes;
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = 100;

    hr = voice->SubmitSourceBuffer(&buffer);

    if (FAILED(hr))
        throw ref new COMException(hr, "SubmitSourceBuffer failure");

    voice->Start(0);
    this->runningVoices[sample->Name] = voice;
}

void UniversalAudioPlayer::Stop(AudioSample^ sample)
{
    if (!this->IsPlaying(sample))
    {
        return;
    }

    auto voice = this->runningVoices[sample->Name];
    voice->Stop();
    voice->FlushSourceBuffers();
    voice->DestroyVoice();
    
    this->runningVoices.erase(sample->Name);
}