#pragma once

#include "pch.h"
#include "UniversalAudioPlayer.h"

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

    if (FAILED(hr))
        ref new COMException(hr, "Could not create mastering voice");

    xAudio->StartEngine();
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

    auto reader = new RiffReader();
    auto data = reader->Read(sample->Buffer);

    IXAudio2SourceVoice * voice = this->CreateVoice(data.waveFormat);
    XAUDIO2_BUFFER buffer = this->CreateAudioBuffer(data);
    HRESULT hr = voice->SubmitSourceBuffer(&buffer);

    if (FAILED(hr))
        throw ref new COMException(hr, "Could not submit buffer");

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

IXAudio2SourceVoice* UniversalAudioPlayer::CreateVoice(WAVEFORMATEX* wavFormat)
{
    IXAudio2SourceVoice * voice = NULL;
    HRESULT hr = xAudio->CreateSourceVoice(&voice, wavFormat);

    if (FAILED(hr))
        throw ref new COMException(hr, "CreateSourceVoice failure");

    return voice;
}

XAUDIO2_BUFFER UniversalAudioPlayer::CreateAudioBuffer(AudioData data)
{
    XAUDIO2_BUFFER buffer = { 0 };
    buffer.AudioBytes = data.numberOfBytes;
    buffer.pAudioData = data.bytes;
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    
    if (data.loopLength > 0)
    {
        buffer.LoopLength = data.loopLength;
        buffer.LoopBegin = data.loopStart;
    }

    return buffer;
}