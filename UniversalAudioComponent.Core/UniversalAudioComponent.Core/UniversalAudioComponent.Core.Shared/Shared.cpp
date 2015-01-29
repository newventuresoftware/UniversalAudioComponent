#pragma once

#include "pch.h"
#include "Shared.h"

namespace UniversalAudioComponent
{
    byte* GetBufferByteAccess(IBuffer^ buffer)
    {
        Object^ obj = buffer;
        ComPtr<IInspectable> insp(reinterpret_cast<IInspectable*>(obj));

        ComPtr<IBufferByteAccess> bufferByteAccess;
        insp.As(&bufferByteAccess);

        byte* pixels = nullptr;
        bufferByteAccess->Buffer(&pixels);

        return pixels;
    }
}