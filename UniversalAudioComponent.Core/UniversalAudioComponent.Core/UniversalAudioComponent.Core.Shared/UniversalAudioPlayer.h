#pragma once

#include "Chunks.h"
#include "AudioSample.h"
#include "RiffReader.h"
#include "AudioData.h"

using namespace Platform;
using namespace Microsoft::WRL;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation::Collections;
using namespace Concurrency;

namespace UniversalAudioComponent
{
    public ref class UniversalAudioPlayer sealed
    {
    private:

        ComPtr<IXAudio2> xAudio;
        IXAudio2MasteringVoice * masteringVoice;
        IXAudio2SourceVoice * musicVoice;
        IXAudio2SourceVoice * effectsVoice;
        RiffReader* reader;
        std::map<String^, IXAudio2SourceVoice*> runningVoices;

        bool IsPlaying(AudioSample^);

    public:
        UniversalAudioPlayer();
        void Play(AudioSample^ sample);
        void Stop(AudioSample^ sample);
    };
}

