#include "pch.h"
#include "RiffReader.h"

using namespace UniversalAudioComponent;

RiffReader::RiffReader()
{
}

AudioData RiffReader::Read(IBuffer^ buffer)
{
    auto formatChunk = FindChunk(buffer, fourccFMT);
    auto dataChunk = FindChunk(buffer, fourccDATA);
    auto waveFormat = reinterpret_cast<WAVEFORMATEX*>(formatChunk.data);
    
    AudioData data;
    data.bytes = dataChunk.data;
    data.numberOfBytes = dataChunk.size;
    data.waveFormat = waveFormat;

    this->SetLoopData(buffer, data);

    return data;
}

void RiffReader::SetLoopData(IBuffer^ buffer, AudioData& data)
{
    auto sampleChunk = FindChunk(buffer, fourccSAMPLE);
    data.loopLength = 0;
    data.loopStart = 0;

    if (sampleChunk.id != fourccSAMPLE)
    {
        return;
    }

    if (sampleChunk.size < sizeof(RIFFMIDISample))
    {
        return;
    }

    auto midiSample = reinterpret_cast<const RIFFMIDISample*>(sampleChunk.data);
    auto loops = reinterpret_cast<const MIDILoop*>(sampleChunk.data + sizeof(RIFFMIDISample));

    for (unsigned int i = 0; i < midiSample->loopCount; i++)
    {
        if (loops[i].type == MIDILoop::LOOP_TYPE_FORWARD)
        {
            // Return 'forward' loop
            data.loopStart = loops[i].start;
            data.loopLength = loops[i].end + loops[i].start + 1;
        }
    }
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

    info.data = NULL;
    info.id = 0;
    info.size = 0;

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