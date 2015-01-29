#pragma once

#include "pch.h"

using namespace Platform;
using namespace Platform::Collections;
using namespace Microsoft::WRL;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::Streams;

namespace UniversalAudioComponent
{
    byte* GetBufferByteAccess(IBuffer^ buffer);
    void WriteDataToBuffer(Vector<byte>^ buffer, byte * data, DWORD bytes);
    void WriteDataToBuffer(byte * buffer, byte * data, DWORD bytes);
    void WriteDataToBuffer(byte * buffer, Vector<byte>^ data);
}