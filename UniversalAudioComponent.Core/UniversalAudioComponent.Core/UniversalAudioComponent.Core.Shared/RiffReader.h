#pragma once

#include "Chunks.h"
#include "AudioData.h"

using namespace Platform;
using namespace Microsoft::WRL;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation::Collections;
using namespace Concurrency;

namespace UniversalAudioComponent
{

    class RiffReader sealed
    {
    private:
        byte* GetBufferByteAccess(IBuffer^ buffer);
        ChunkInfo FindChunk(IBuffer^ buffer, uint32 chunkId);
        void ReadChunkData(ChunkInfo info, byte * buffer);
    public:
        RiffReader();

        AudioData^ Read(IBuffer^ buffer);
    };
}

