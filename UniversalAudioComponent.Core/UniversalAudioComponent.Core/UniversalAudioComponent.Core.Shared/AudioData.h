#pragma once

#include "pch.h"

using namespace Platform;

namespace UniversalAudioComponent
{
    ref class AudioData sealed
    {
    internal:
        byte * bytes;
        unsigned int numberOfBytes;
        WAVEFORMATEX* waveFormat;

    public:
        AudioData()
        {

        }

    };
}

