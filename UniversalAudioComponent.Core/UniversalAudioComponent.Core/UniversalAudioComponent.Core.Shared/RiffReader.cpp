#include "pch.h"
#include "RiffReader.h"

using namespace UniversalAudioComponent;

RiffReader::RiffReader()
{
}

AudioData^ RiffReader::Read(IBuffer^ buffer)
{
    auto riffChunk = FindChunk(buffer, fourccRIFF);
    auto formatChunk = FindChunk(buffer, fourccFMT);
    auto dataChunk = FindChunk(buffer, fourccDATA);

    auto waveFormat = reinterpret_cast<WAVEFORMATEX*>(formatChunk.data);

    auto data = ref new AudioData();
    data->bytes = dataChunk.data;
    data->numberOfBytes = dataChunk.size;
    data->waveFormat = waveFormat;

    return data;
}

ChunkInfo RiffReader::FindChunk(IBuffer^ buffer, uint32 chunkId)
{
    byte* ptr = GetBufferByteAccess(buffer);
    byte* endPtr = ptr + (sizeof(byte) * (buffer->Length));
    ChunkInfo info;

    while (ptr < endPtr)
    {
        auto chunk = reinterpret_cast<RiffChunk*>(ptr);
        info.id = chunk->tag;
        info.size = chunk->size;
        ptr += sizeof(RiffChunk);

        if (chunk->tag == fourccRIFF)
        {
            auto type = reinterpret_cast<uint32*>(ptr);
            info.size = 4;
        }

        info.data = ptr;

        ptr += (info.size * sizeof(byte));

        if (info.id == chunkId)
        {
            return info;
        }

    }

    return info;
}

byte* RiffReader::GetBufferByteAccess(IBuffer^ buffer)
{
    Object^ obj = buffer;
    ComPtr<IInspectable> insp(reinterpret_cast<IInspectable*>(obj));

    ComPtr<IBufferByteAccess> bufferByteAccess;
    insp.As(&bufferByteAccess);

    byte* pixels = nullptr;
    bufferByteAccess->Buffer(&pixels);

    return pixels;
}