#pragma once

#include "pch.h"

using namespace Platform;

namespace UniversalAudioComponent
{
    struct AudioData
    {
        byte * bytes;
        unsigned int numberOfBytes;
        unsigned int loopStart;
        unsigned int loopLength;
        WAVEFORMATEX* waveFormat;
    };
}

