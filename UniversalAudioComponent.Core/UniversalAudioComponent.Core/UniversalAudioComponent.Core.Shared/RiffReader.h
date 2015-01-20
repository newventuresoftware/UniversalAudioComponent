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
        ChunkInfo FindChunk(IBuffer^ buffer, uint32 fourcc);
        byte* GetBufferByteAccess(IBuffer^ buffer);
    public:
        RiffReader();

        AudioData Read(IBuffer^ buffer);
    };
}