#pragma once

#include "Chunks.h"
#include "AudioSample.h"
#include "RiffReader.h"
#include "AudioData.h"

using namespace Platform;
using namespace Platform::Collections;
using namespace Microsoft::WRL;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::Streams;

namespace UniversalAudioComponent
{
    public ref class AudioDecoder sealed
    {
    public:
        AudioDecoder();
        Array<byte>^ Decode(IRandomAccessStream^ audioStream);

    private:
        
        ComPtr<IMFMediaType> ConfigureAudioStream(ComPtr<IMFSourceReader> reader);
        Vector<byte>^ GetDecodedAudioData(ComPtr<IMFSourceReader> reader);
        Array<byte>^ DecodeAsWav(ComPtr<IMFSourceReader> reader);
        Array<byte>^ GetWav(Vector<byte>^ audioBytes, ComPtr<IMFMediaType> audioType);

        unsigned int WriteHeader(byte * bufferPtr, ComPtr<IMFMediaType> audioType);
        unsigned int WriteFormat(byte * bufferPtr, ComPtr<IMFMediaType> audioType);
    };
}

