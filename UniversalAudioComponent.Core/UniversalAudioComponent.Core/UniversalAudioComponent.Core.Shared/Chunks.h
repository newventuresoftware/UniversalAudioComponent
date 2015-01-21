#pragma once

#include "pch.h"

//Little-Endian
const uint32 fourccRIFF = 'FFIR';
const uint32 fourccWAVE = 'EVAW';
const uint32 fourccXWMA = 'AMWX';
const uint32 fourccDATA = 'atad';
const uint32 fourccFMT = ' tmf';
const uint32 fourccSAMPLE = 'lpms';

private struct RiffChunk
{
    uint32 tag;
    uint32 size;
};

private struct ChunkInfo
{
    uint32 id;
    uint32 size;
    byte* data;
};

struct RIFFMIDISample
{
    uint32_t        manufacturerId;
    uint32_t        productId;
    uint32_t        samplePeriod;
    uint32_t        unityNode;
    uint32_t        pitchFraction;
    uint32_t        SMPTEFormat;
    uint32_t        SMPTEOffset;
    uint32_t        loopCount;
    uint32_t        samplerData;
};

struct MIDILoop
{
    static const uint32_t LOOP_TYPE_FORWARD = 0x00000000;
    static const uint32_t LOOP_TYPE_ALTERNATING = 0x00000001;
    static const uint32_t LOOP_TYPE_BACKWARD = 0x00000002;

    uint32_t cuePointId;
    uint32_t type;
    uint32_t start;
    uint32_t end;
    uint32_t fraction;
    uint32_t playCount;
};


