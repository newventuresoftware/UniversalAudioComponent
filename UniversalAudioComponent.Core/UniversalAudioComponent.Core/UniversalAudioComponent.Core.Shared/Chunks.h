#pragma once

#include "pch.h"

const uint32 fourccRIFF = 'FFIR';
const uint32 fourccWAVE = 'EVAW';
const uint32 fourccXWMA = 'AMWX';
const uint32 fourccDATA = 'atad';
const uint32 fourccFMT = ' tmf';

private struct RiffChunk
{
    uint32 tag;
    uint32 size;
};

private struct ChunkInfo
{
    uint32 id;
    uint32 size;
    byte* dataOffset;
};

