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

    auto waveFormat = reinterpret_cast<WAVEFORMATEX*>(formatChunk.dataOffset);

    BYTE * pDataBuffer = new BYTE[dataChunk.size];
    ReadChunkData(dataChunk, pDataBuffer);

    auto data = ref new AudioData();
    data->bytes = pDataBuffer;
    data->numberOfBytes = dataChunk.size;
    data->waveFormat = waveFormat;

    return data;
}

byte* RiffReader::GetBufferByteAccess(IBuffer^ buffer)
{
    // Cast to Object^, then to its underlying IInspectable interface.

    Object^ obj = buffer;
    ComPtr<IInspectable> insp(reinterpret_cast<IInspectable*>(obj));

    // Query the IBufferByteAccess interface.
    ComPtr<IBufferByteAccess> bufferByteAccess;
    insp.As(&bufferByteAccess);

    // Retrieve the buffer data.

    byte* pixels = nullptr;
    bufferByteAccess->Buffer(&pixels);

    return pixels;

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
            //ptr += sizeof(uint32);
            info.size = 4;
        }

        info.dataOffset = ptr;

        ptr += (info.size * sizeof(byte));

        if (info.id == chunkId)
        {
            return info;
        }

    }

    return info;
}

void RiffReader::ReadChunkData(ChunkInfo info, byte * buffer)
{
    byte* ptr = info.dataOffset;
    byte* endPtr = ptr + (sizeof(byte) * (info.size));

    while (ptr < endPtr)
    {
        *buffer = *ptr;

        ptr += sizeof(byte);
        buffer += sizeof(byte);
    }
}
