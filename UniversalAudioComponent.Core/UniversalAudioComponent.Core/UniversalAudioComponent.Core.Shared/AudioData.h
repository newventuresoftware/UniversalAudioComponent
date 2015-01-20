#pragma once

#include "pch.h"

using namespace Platform;

namespace UniversalAudioComponent
{
    struct AudioData
    {
        byte * bytes;
        unsigned int numberOfBytes;
        WAVEFORMATEX* waveFormat;
    };
}

