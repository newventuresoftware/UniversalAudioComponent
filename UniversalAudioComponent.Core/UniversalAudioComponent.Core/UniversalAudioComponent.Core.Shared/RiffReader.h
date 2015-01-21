#pragma once

#include "Chunks.h"
#include "AudioData.h"

using namespace Microsoft::WRL;
using namespace Windows::Storage::Streams;

namespace UniversalAudioComponent
{
    class RiffReader sealed
    {
    private:
        ChunkInfo FindChunk(IBuffer^ buffer, uint32 fourcc);
        byte* GetBufferByteAccess(IBuffer^ buffer);
        void SetLoopData(IBuffer^ buffer, AudioData& data);
    public:
        RiffReader();

        AudioData Read(IBuffer^ buffer);
    };
}