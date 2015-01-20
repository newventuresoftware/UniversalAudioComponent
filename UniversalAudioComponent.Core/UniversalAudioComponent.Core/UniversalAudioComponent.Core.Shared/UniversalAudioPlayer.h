#pragma once

#include "Chunks.h"
#include "AudioSample.h"
#include "RiffReader.h"
#include "AudioData.h"

using namespace Platform;
using namespace Microsoft::WRL;
using namespace Windows::Foundation::Collections;

namespace UniversalAudioComponent
{
    public ref class UniversalAudioPlayer sealed
    {
    private:

        ComPtr<IXAudio2> xAudio;
        IXAudio2MasteringVoice * masteringVoice;
        std::map<String^, IXAudio2SourceVoice*> runningVoices;

        bool IsPlaying(AudioSample^);
        IXAudio2SourceVoice* CreateVoice(WAVEFORMATEX* wavFormat);
        XAUDIO2_BUFFER CreateAudioBuffer(AudioData data);

    public:
        UniversalAudioPlayer();
        void Play(AudioSample^ sample);
        void Stop(AudioSample^ sample);
    };
}

