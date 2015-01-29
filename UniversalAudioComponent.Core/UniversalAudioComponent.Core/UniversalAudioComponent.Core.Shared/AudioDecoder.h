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

        void WriteDataToBuffer(Vector<byte>^ buffer, byte * data, DWORD bytes);
        void WriteDataToBuffer(byte * buffer, byte * data, DWORD bytes);
        void WriteDataToBuffer(byte * buffer, Vector<byte>^ data);
        Array<byte>^ DecodeAsWav(ComPtr<IMFSourceReader> reader);
        Array<byte>^ GetWav(Vector<byte>^ audioBytes, ComPtr<IMFMediaType> audioType);
    };
}

