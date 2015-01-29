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

    void WriteDataToBuffer(Vector<byte>^ buffer, byte * data, DWORD bytes)
    {
        for (DWORD i = 0; i < bytes; i++)
        {
            buffer->Append(*data);
            data++;
        }
    }

    void WriteDataToBuffer(byte * buffer, byte * data, DWORD bytes)
    {
        for (DWORD i = 0; i < bytes; i++)
        {
            *buffer = *data;
            data++;
            buffer++;
        }
    }

    void WriteDataToBuffer(byte * buffer, Vector<byte>^ data)
    {
        for (unsigned int i = 0; i < data->Size; i++)
        {
            *buffer = data->GetAt(i);
            buffer++;
        }
    }
}